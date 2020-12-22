#include "SAT3D.h"
#include "../CollisionDetection.h"

bool Physics::SAT3D::overlap(const Projection& a, const Projection& b) const
{
	return !(b[1] < a[0] || a[1] < b[0]);
}

float Physics::SAT3D::getOverlap(const Projection& a, const Projection& b) const
{
	return fminf(a[1], b[1]) - fmaxf(a[0], b[0]);
}

void Physics::SAT3D::testNormals(ColliderSAT*& aSAT, ColliderSAT*& bSAT, std::list<glm::vec3>& normals, CollisionManfold& res) const
{
	for (auto itt = normals.begin(); itt != normals.end();) {
		Projection a = aSAT->project(*itt);
		Projection b = bSAT->project(*itt);
		aSAT->setCache(*itt);
		if (!overlap(a, b)) {
			res.collided = false;
			aSAT->setCache(*itt);
			break;
		}
		float overlap = getOverlap(a, b);
		if (overlap < res.penetration) {
			res.penetration = overlap;
			res.normal = *itt;
		}
		itt = normals.erase(itt);
	}
	normals.clear();
}

void Physics::SAT3D::bestEdge(ColliderSAT* coll, const glm::vec3& n) const
{
	unsigned index = 0;
	glm::vec3 v = coll->getFarthest(n, index);
	index = coll->rawToIndices(index);
	glm::vec3 v1 = coll->getVertex(index + 1);
	glm::vec3 v2 = coll->getVertex(index - 1);

	glm::vec3 l = glm::normalize(v - v1);
	glm::vec3 r = glm::normalize(v - v2);

	if (glm::dot(r, n) <= glm::dot(l, n)) {
		// edge v2 v
	}
	else {
		// edge v, v1
	}
}

const Physics::CollisionManfold Physics::SAT3D::getCollisionData(Collider* a, Collider* b)
{
	ColliderSAT* aSAT = reinterpret_cast<ColliderSAT*>(a);
	ColliderSAT* bSAT = reinterpret_cast<ColliderSAT*>(b);
	CollisionManfold res;
	res.collided = true;
	res.bodies = {
		a, b
	};
	std::list<glm::vec3> normals = aSAT->getAllNormals(true, true);

	res.penetration = INFINITY;
	testNormals(aSAT, bSAT, normals, res);

	if (!res.collided)
		return res;

	normals = bSAT->getAllNormals(true, true);
	testNormals(aSAT, bSAT, normals, res);

	return res;
}
