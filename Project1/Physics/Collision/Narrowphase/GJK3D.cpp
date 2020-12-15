#include "GJK3D.h"
#include "../CollisionDetection.h"
#include <gtx/string_cast.hpp>

bool sameDirection(const glm::vec3& a, const glm::vec3& b) {
	return glm::dot(a, b) > 0;
}

Physics::SupportPoint support(const Physics::Collider* a, const Physics::Collider* b, const glm::vec3& dir) {
	Physics::SupportPoint res;
	res.a = a->support(dir);
	res.b = b->support(-dir);
	res.v = res.a - res.b;
	////////////////////////////////////////////
	// res.a = a->support(-dir);
	// res.b = b->support(dir);
	// res.v = res.b - res.a;
	return res;
}
Physics::GJK3D::Triangle* Physics::GJK3D::getClosest(std::list<Triangle>& faces, float& dist)
{
	dist = INFINITY;
	float d = 0;
	Triangle* closest_face = nullptr;
	for (Triangle& face : faces) {
		d = glm::dot(face.a.v, face.n);
		if (d < dist) {
			dist = d;
			closest_face = &face;
		}
	}
	return closest_face;
}

glm::vec3 ClipFunc(const Physics::GJK3D::Triangle& triangle)
{
	glm::vec3 res(0);
	const glm::vec3 p = triangle.n * glm::dot(triangle.n, triangle.a.v);
	const glm::vec3& a = triangle.a.v;
	const glm::vec3& b = triangle.b.v;
	const glm::vec3& c = triangle.c.v;
	const glm::vec3
		v0 = b - a,
		v1 = c - a,
		v2 = p - a;
	const float d00 = glm::dot(v0, v0);
	const float d01 = glm::dot(v0, v1);
	const float d11 = glm::dot(v1, v1);
	const float d20 = glm::dot(v2, v0);
	const float d21 = glm::dot(v2, v1);

	const float denom = d00 * d11 - d01 * d01;

	res.y = (d11 * d20 - d01 * d21) / denom;
	res.z = (d00 * d21 - d01 * d20) / denom;
	res.x = 1.0f - res.y - res.z;
	return res;
}
void determinCollisionData(Physics::CollisionManfold& info, const Physics::GJK3D::Triangle* face, const float& penertration) {

	info.collided = true;
	info.normal = face->n;
	info.penetration = penertration;
	glm::vec3 barr = ClipFunc(*face);
	const Physics::Collider* a = info.bodies[0];
	const Physics::Collider* b = info.bodies[1];
	info.points[0] =
		barr.x * ((glm::sign(face->a.dir) * *a->scale) + *a->position) +
		barr.y * ((glm::sign(face->b.dir) * *a->scale) + *a->position) +
		barr.z * ((glm::sign(face->c.dir) * *a->scale) + *a->position);

	info.points[1] =
		barr.x * ((glm::sign(-face->a.dir) * *b->scale) + *b->position) +
		barr.y * ((glm::sign(-face->b.dir) * *b->scale) + *b->position) +
		barr.z * ((glm::sign(-face->c.dir) * *b->scale) + *b->position);
	/*info.points[0] =
		barr.x * face->a.a +
		barr.y * face->b.a +
		barr.z * face->c.a;

	info.points[1] =
		barr.x * face->a.b +
		barr.y * face->b.b +
		barr.z * face->c.b;*/
	assert(!glm::isnan(info.penetration) && !glm::isinf(info.penetration) && "Cant determin collision info");
}

void Physics::GJK3D::EPA(Physics::CollisionManfold& info, Simplex& simplex) {
#define EPA_TOLERANCE 0.0001f
#define EPA_MAX_NUM_FACES 64
#define EPA_MAX_NUM_LOOSE_EDGES 32
#define EPA_MAX_NUM_ITERATIONS 64
#define EPA_BIAS 0.000001f

	std::list<Triangle> faces;
	const SupportPoint& a = simplex.a();
	const SupportPoint& b = simplex.b();
	const SupportPoint& c = simplex.c();
	const SupportPoint& d = simplex.d();
	faces.emplace_back(a, b, c);
	faces.emplace_back(a, c, d);
	faces.emplace_back(a, d, b);
	faces.emplace_back(b, d, c);
	Triangle* closest_face = nullptr;
	std::list<Edge> loose_edges;
	for(short itt = 0; itt < EPA_MAX_NUM_ITERATIONS; itt++) {
		loose_edges.clear();
		float min = 0;
		if (itt == EPA_MAX_NUM_ITERATIONS - 1) {
			int t = 0;
		}
		closest_face = getClosest(faces, min);
		if (!closest_face)
			return;

		const glm::vec3 dir = closest_face->n;
		const glm::vec3 p = support(info.bodies[0], info.bodies[1], dir).v;
		const SupportPoint p_raw = support(info.bodies[0], info.bodies[1], dir);

		if (glm::dot(p, dir) - min < EPA_TOLERANCE) {
			determinCollisionData(info, closest_face, glm::dot(p, dir));
			// std::cout << glm::to_string(info.points[0]) << " : " << glm::to_string(info.points[1]) << /*glm::to_string(*info.bodies[1]->position) <<*/ std::endl;
			return;
		}
		for (unsigned k = 0; k < faces.size(); k++) {
			Triangle& face = Utils::elementAt(faces, k);
			if (sameDirection(face.n, p - face.a.v)) {
				const std::vector<const SupportPoint*> points = face.all();
				for (char i = 0; i < 3; i++) {
					Edge current_edge = { *points[i], *points[(i + 1) % 3] };
					bool found_edge = false;
					for (Edge& loose_edge : loose_edges) {
						if (loose_edge == current_edge) {
							loose_edge = loose_edges.back();
							loose_edges.pop_back();
							found_edge = true;
							break;
						}
					}
					if (!found_edge) {
						assert(loose_edges.size() < EPA_MAX_NUM_LOOSE_EDGES);
						loose_edges.push_back(current_edge);
					}
				}
				std::swap(face, faces.back());
				faces.pop_back();
				k--;
			}
		}
		for (Edge& edge : loose_edges) {
			assert(faces.size() < EPA_MAX_NUM_FACES);
			faces.emplace_back(edge.a, edge.b, p_raw);
			Triangle& back = faces.back();
			back.n = glm::normalize(glm::cross(edge.a.v - edge.b.v, edge.a.v - p));
			assert(!glm::any(glm::isnan(back.n)));
			if (glm::dot(back.a.v, back.n) + EPA_BIAS < 0) {
				const glm::vec3 t = back.a.v;
				back.a = back.b;
				back.b = t;
				back.n *= -1;
			}
		}
	}
	// EPA failed
	/*info.collided = true;
	info.normal = closest_face->n;
	info.penertraion = glm::dot(closest_face->a.v, closest_face->n);
	glm::vec3 barr = ClipFunc(*closest_face, info.penertraion);
	info.points[0] =
		barr.x * closest_face->a.a +
		barr.y * closest_face->b.a +
		barr.z * closest_face->c.a;
	info.points[1] = info.points[0];*/
	determinCollisionData(info, closest_face, glm::dot(closest_face->a.v, closest_face->n));
}
const Physics::CollisionManfold Physics::GJK3D::getCollisionData(Collider* a, Collider* b)
{
#define GJK_MAX_ITTERATION 256
	CollisionManfold res;
	res.collided = false;
	res.bodies = {
		a, b
	};

	Simplex simplex;
	glm::vec3 dir = Utils::xAxis();
	simplex.push(support(a, b, dir));
	dir *= -1;

	glm::vec3 ao(0);
	glm::vec3 ab(0);
	glm::vec3 ac(0);
	glm::vec3 ad(0);
	unsigned counter = 0;
	while (counter < GJK_MAX_ITTERATION) {
		counter++;
		simplex.push(support(a, b, dir));

		ao = -simplex.a().v;
		ab = simplex.b() - simplex.a();
		ac = simplex.c() - simplex.a();
		ad = simplex.d() - simplex.a();
		
		switch (simplex.size)
		{
		case 2: // line segment
			if (!sameDirection(-ao, dir)) {
				// simplex doesnt contain origin
				return res;
			}
			// simplex could contain the origin

			dir = Utils::tripProduct(ab, ao, ab); // normal of line segment directed towards the originart();
			if (dir == Utils::zero()) { //origin is on this line segment
				//Apparently any normal search vector will do?
				dir = glm::cross(ab, Utils::xAxis()); //normal with x-axis
				if (dir == Utils::zero())
					dir = glm::cross(ab, Utils::zAxis(-1)); //normal with z-axis
			}
			break;
		case 3: // triangle
			glm::vec3 n = glm::cross(ab, ac);
			if (sameDirection(glm::cross(ab, n), ao)) {
				// simplex doesnt contain origin
				dir = Utils::tripProduct(ab, ao, ab);
				simplex.pop();
			}
			else if (sameDirection(glm::cross(n, ac), ao)) {
				dir = Utils::tripProduct(ac, ao, ac);
				simplex.pop();
				simplex.b() = simplex.c();
			}
			else if (sameDirection(n, ao)) {
				dir = n;
			}
			else {
				simplex = Simplex({ simplex.a(), simplex.c(), simplex.b() });
				dir = -n;
			}
			break;
		case 4: // pyramid
			const glm::vec3 abc = glm::cross(ab, ac); // normals of the triangles
			const glm::vec3 acd = glm::cross(ac, ad); // normals of the triangles
			const glm::vec3 adb = glm::cross(ad, ab); // normals of the triangles

			if (sameDirection(abc, ao)) {
				simplex.pop();
				dir = abc;
			}
			else if (sameDirection(acd, ao)) {
				simplex = Simplex({ simplex.a(), simplex.c(), simplex.d() });
				dir = acd;
			}
			else if (sameDirection(adb, ao)) {
				simplex = Simplex({ simplex.a(), simplex.d(), simplex.b() });
				dir = adb;
			}
			else {
				EPA(res, simplex);
				// res.collided = true;
				return res;
			}
			break;
		}
	}

	return res;
}
