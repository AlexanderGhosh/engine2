#include "GJK.h"
#include "../CollisionDetection.h"

Physics::GJK::Simplex::Simplex(const std::vector<SupportPoint>& inps) : Simplex()
{
	std::vector<SupportPoint> a = inps;
	if (a.size() > 4)
		a.resize(4);
	size = a.size();
	for (char i = 0; i < size; i++) {
		values[i] = a[i];
	}
}

const Physics::CollisionManfold Physics::GJK::getCollisionData(Collider* a_, Collider* b_)
{
	CollisionManfold info;
	info.bodies[0] = a_;
	info.bodies[1] = b_;

	Simplex sim;
	sim.clear();
	glm::vec3 dir = Utils::xAxis(); // choose search dir
	SupportPoint s = support(a_, b_, dir);
	/*if (fabs(glm::dot(dir, s.v) >= glm::length(s.v) * 0.8f)) {
		dir = Helpers::YAxis();
		s = support(a_, b_, dir);
	}*/
	sim.push(s);
	// dir = -s.v;
	dir = -dir;

	const unsigned int _EXIT_ITERATION_LIMIT = 75;
	unsigned int _EXIT_ITERATION_NUM = 0;

	while (true)
	{
		if (_EXIT_ITERATION_NUM++ >= _EXIT_ITERATION_LIMIT)
			return info;
		SupportPoint a = support(a_, b_, dir);

		if (glm::dot(a.v, dir) <= 0)
			return info;
		sim.push(a);

		const glm::vec3 ao = -sim[0].v;
		glm::vec3 ab = sim[1].v - sim[0].v;
		glm::vec3 ac = sim[2].v - sim[0].v;
		const glm::vec3 ad = sim[3].v - sim[0].v;

		if (sim.size == 2) {
			// simplex is a_ line, being here means that the early out was passed, and thus
			// the origin must be between point a_ and b_
			// search direction is perpendicular to ab and coplanar with ao
			// normal of line segment
			dir = Utils::tripCross(ab, ao, ab);
		}
		else if (sim.size == 3) {
			// simplex is a_ triangle, meaning that the origin must be
			// normal of the triangle
			const glm::vec3 n = glm::cross(ab, ac);
			// gjk_simtest(glm::cross(ab, n), ao)
			if (gjk_simtest(-ac, ao)) {
				// origin is outside the triangle, near the edge ab
				// reset the simplex to the line ab and continue
				// search direction is perpendicular to ab and coplanar with ao
				sim = Simplex({ sim[0], sim[1] });
				dir = Utils::tripCross(ab, ao, ab);
				continue;
			}

			if (gjk_simtest(-ab, ao)) {
				// origin is outside the triangle, near the edge ac
				// reset the simplex to the line ac and continue
				// search direction is perpendicular to ac and coplanar with ao
				sim = Simplex({ sim[0], sim[2] });
				dir = Utils::tripCross(ac, ao, ac);
				continue;
			}

			// origin is within the triangular prism defined by the triangle
			// determine if it is above or below
			if (gjk_simtest(n, ao)) {
				// origin is above the triangle, so the simplex is not modified,
				// the search direction is the triangle's face normal
				dir = n;
				continue;
			}

			// origin is below the triangle, so the simplex is rewound the opposite direction
			// the search direction is the new triangle's face normal
			sim = Simplex({sim[0], sim[2], sim[1] });
			dir = -n;
			continue;
		}
		else {
			// the simplex is a_ tetrahedron, must check if it is outside any of the side triangles, (abc, acd, adb)
					// if it is then set the simplex equal to that triangle and continue, otherwise we know
					// there is an intersection and exit

					// check the triangles (abc,acd,adb), scoped as the temporary variables used here
					// will no longer be valid afterward

			glm::vec3 ab = sim[1].v - sim[0].v;
			glm::vec3 ac = sim[2].v - sim[0].v;

			glm::vec3 ad = sim[3].v - sim[0].v;

			if (gjk_simtest(glm::cross(ab, ac), ao)) {
				// origin is in front of triangle abc, simplex is already what it needs to be
				// go to jmp_face
			}
			else if (gjk_simtest(glm::cross(ac, ad), ao)) {
				// origin is in front of triangle acd, simplex is set to this triangle
				// go to jmp_face
				sim = Simplex({ sim[0], sim[2], sim[3] });
			}
			else if (gjk_simtest(glm::cross(ad, ab), ao)) {
				// origin is in front of triangle adb, simplex is set to this triangle
				// go to jmp_face
				sim = Simplex({ sim[0], sim[3], sim[1] });
			}
			else {
				// intersection confirmed, break from the loop
				break;
			}
			ab = sim[1].v - sim[0].v;
			ac = sim[2].v - sim[0].v;

			if (gjk_simtest(-ac, ao)) {
				sim = Simplex({ sim[0], sim[1] });
				dir = Utils::tripCross(ab, ao, ab);
				continue;
			}

			if (gjk_simtest(-ab, ao)) {
				sim = Simplex({ sim[0], sim[2] });
				dir = Utils::tripCross(ac, ao, ac);
				continue;
			}

			sim = Simplex({ sim[0], sim[1], sim[2] });
			dir = glm::cross(ab, ac);
		}
	}
	// simplex now generated

	EPA(info, sim);
	return info;
}

void Physics::GJK::EPA(CollisionManfold& info, Simplex& simplex)
{
	static const float _EXIT_THRESHOLD = 0.001f;
	static const unsigned int _EXIT_ITERATION_LIMIT = 50;
	unsigned int _EXIT_ITERATION_CUR = 0;

	std::list<Triangle> triangles;
	std::list<Edge> edges;

	triangles.emplace_back(simplex[0], simplex[1], simplex[2]);
	triangles.emplace_back(simplex[0], simplex[2], simplex[3]);
	triangles.emplace_back(simplex[0], simplex[3], simplex[1]);
	triangles.emplace_back(simplex[1], simplex[3], simplex[2]);

	while (true) {
		if (_EXIT_ITERATION_CUR++ >= _EXIT_ITERATION_LIMIT) 
			return;

		// closest triangle to origin
		Triangle triangle = *triangles.begin();

		float dist = INFINITY;
		for (Triangle& tri : triangles) {
			float dst = glm::abs(glm::dot(tri.n, -tri.a.v));
			if (dst < dist) {
				dist = dst;
				triangle = tri;
			}
		}

		// get the next support point in front of the current triangle, away from the origin
		const SupportPoint supp = support(info.bodies[0], info.bodies[1], triangle.n);

		// checks how much further this new point will takes us from the origin
		// if it is not far enough then we assume we have found the closest triangle
		// on the hull from the origin
		const float d = glm::dot(triangle.n, supp.v);
		if (d - dist < _EXIT_THRESHOLD) {
			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			// GENERATE CONTACT INFO AND RETURN


			info.collided = true;
			extrapolateContactInformation(triangle, info);
			info.normal = triangle.n;
			info.penertraion = d;
			return;
		}

		for (auto it = triangles.begin(); it != triangles.end();) {
			// can this face be 'seen' by the support?
			if (glm::dot(it->n, supp.v - it->a.v) > 0) {
				addEdge(it->a, it->b, edges);
				addEdge(it->b, it->c, edges);
				addEdge(it->c, it->a, edges);
				it = triangles.erase(it);
				continue;
			}
			it++;
		}

		// create new triangles from the edges in the edge list
		for (auto it = edges.begin(); it != edges.end(); it++) {
			triangles.emplace_back(supp, it->a, it->b);
		}

		edges.clear();
	}

	return;
}

void Physics::GJK::ClipFunc(const Triangle& triangle, CollisionManfold& info)
{

}

void Physics::GJK::extrapolateContactInformation(const Triangle& triangle, CollisionManfold& info)
{
	float dist = INFINITY;
	SupportPoint closestPoint;
	for (const auto& point : triangle.all()) {
		const float d = glm::length(point->v);
		if (d < dist) {
			dist = d;
			closestPoint = *point;
		}
	}
	if (dist == INFINITY) 
		return;
	info.points[0] = closestPoint.a;
	info.points[1] = closestPoint.b;
	info.points[0] = { 0, 0, -5 };
	info.points[1] = { 0, -dist, -5 };
	info.normal = glm::normalize(info.points[0] - info.points[1]);
	info.collided = true;
	info.penertraion = dist;
}
