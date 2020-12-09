#include "GJK3D.h"
#include "../CollisionDetection.h"

bool sameDirection(const glm::vec3& a, const glm::vec3& b) {
	return glm::dot(a, b) > 0;
}

Physics::SupportPoint support(const Physics::Collider*& a, const Physics::Collider*& b, const glm::vec3& dir) {
	Physics::SupportPoint res;
	res.a = a->support(dir);
	res.b = b->support(-dir);
	res.v = res.a - res.b;
	return res;
}
Physics::GJK3D::Triangle* Physics::GJK3D::getClosest(std::list<Triangle>& faces, float& dist)
{
	dist = glm::dot(faces.front().a.v, faces.front().n);
	float d = 0;
	Triangle* closest_face = &faces.front();;
	for (Triangle& face : faces) {
		d = glm::dot(face.a.v, face.n);
		if (d < dist) {
			dist = d;
			closest_face = &face;
		}
	}
	return closest_face;
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

	for(short itt = 0; itt < EPA_MAX_NUM_ITERATIONS; itt++) {
		float min = glm::dot(a.v, c.v);
		Triangle* closest_face = getClosest(faces, min);

		const glm::vec3 dir = closest_face->n;
		const glm::vec3 p = support(info.bodies[0], info.bodies[1], dir).v;
		const SupportPoint p_raw = support(info.bodies[0], info.bodies[1], dir);

		if (glm::dot(p, dir) - min < EPA_TOLERANCE) {
			info.collided = true;
			info.normal = closest_face->n;
			info.penertraion = glm::dot(p, dir);
			return;
		}
		std::list<Edge> loose_edges;
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
				faces.remove(face);
				k--;
			}
		}
		for (Edge& edge : loose_edges) {
			assert(faces.size() < EPA_MAX_NUM_FACES);
			faces.emplace_back(edge.a, edge.b, p_raw);
			Triangle& back = faces.back();
			back.n = glm::normalize(glm::cross(edge.a.v - edge.b.v, edge.a.v - p));

			if (glm::dot(back.a.v, back.n) + EPA_BIAS < 0) {
				const glm::vec3 t = back.a.v;
				back.a = back.b;
				back.b = a;
				back.n *= -1;
			}
		}
		// EPA failed
		info.collided = true;
		info.normal = closest_face->n;
		info.penertraion = glm::dot(closest_face->a.v, closest_face->n);
	}
}
const Physics::CollisionManfold Physics::GJK3D::getCollisionData(Collider* a, Collider* b)
{
	CollisionManfold res;
	res.collided = false;
	res.bodies = {
		a, b 
	};

	Simplex simplex;
	glm::vec3 dir = Utils::xAxis();
	simplex.push(support(a, b, dir));
	dir *= -1;
	while (true) {
		simplex.push(support(a, b, dir));

		const glm::vec3 ao = -simplex.a().v;
		const glm::vec3 ab = simplex.b() - simplex.a();
		const glm::vec3 ac = simplex.c() - simplex.a();
		const glm::vec3 ad = simplex.d() - simplex.a();

		switch (simplex.size)
		{
		case 2: // line segment
			if (!sameDirection(-ao, dir)) {
				// simplex doesnt contain origin
				return res;
			}
			// simplex could contain the origin
			dir = Utils::tripProduct(ab, ao, ab); // normal of line segment directed towards the origin
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
				return res;
			}
			break;
		}
	}

	return res;
}
