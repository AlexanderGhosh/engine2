#include "SAT3D.h"
#include "../CollisionDetection.h"
#include <gtx/string_cast.hpp>

#define SHAPE Utils::Shapes
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

Utils::Shapes::Face<glm::vec3, 4> Physics::SAT3D::bestFace(ColliderSAT* coll, const glm::vec3& n) const
{
	// 3D implementaion
	unsigned index = 0;
	glm::vec3 v = coll->getFarthest(n, index, true);
	// index = coll->rawToIndices(index);
	std::list<glm::vec3> faces = coll->getAllFaces(index, false, true);
	SHAPE::Face<glm::vec3, 4> face;
	float val = 0;
	for (auto itt = faces.begin(); itt != faces.end();) {
		glm::vec3 norm = coll->getNormal((*itt).x, true);
		norm = norm * *coll->rotation;
		//glm::vec3 norm = Utils::yAxis();
		float t = fabsf(glm::dot(glm::normalize(n), glm::normalize(norm)));
		if (t > val) {
			val = t;
			face.set({ *(itt++), *(itt++), *(itt++), *(itt++) });
		}
		else {
			std::advance(itt, 4);
		}
	}
	// for (unsigned i = 0; i < faces.size(); i += 4) {
	// 	glm::vec3 norm = coll->getNormal(faces, true, true);
	// 	float t = fabsf(glm::dot(glm::normalize(n), glm::normalize(norm)));
	// 	if (t > val) {
	// 		val = t;
	// 		best = n;
	// 	}
	// }
	faces.clear();
	for (char i = 0; i < face.getSize(); i++) {
		face[i] = coll->getVertex(face[i].x, true) * *coll->scale * glm::toMat3(*coll->rotation) + *coll->position;
	}
	return face;
}

Physics::SAT3D::ClippedPoints Physics::SAT3D::getContactData(CollisionManfold& info) const {
	using namespace SHAPE;
	ColliderSAT* a = reinterpret_cast<ColliderSAT*>(info.bodies[0]);
	ColliderSAT* b = reinterpret_cast<ColliderSAT*>(info.bodies[1]);

	const glm::vec3& n = info.normal;

	Face<glm::vec3, 4> f1 = bestFace(a, n);
	Face<glm::vec3, 4> f2 = bestFace(b, -n);

	bool fliped = true;

	Face<glm::vec3, 4> ref = f2, inc = f1;
	ColliderSAT* r = b, *i = a;
	glm::vec3 n0 = f1.getNormal();
	glm::vec3 n1 = f2.getNormal(); 
	glm::vec3 refn = n1;
	if (fabsf(glm::dot(n0, n)) > fabsf(glm::dot(n1, n))) {
		ref = f1;
		inc = f2;
		r = a;
		refn = n0;
		fliped = false;
	}

	// begining the clipping

	std::list <Face<glm::vec3, 4>> clippingPlanes;
	auto t = ref.getData<std::list<glm::vec3>>();
	std::list<std::list<glm::vec3>> adjacent = r->getAdjacentFaces(t);
	for (const std::list<glm::vec3>& adj : adjacent) {
		if (t == adj)
			continue;
		clippingPlanes.push_back(Face<glm::vec3, 4>(adj));
	}
	
	ClippedPoints cp = inc.getData<ClippedPoints>();
	for (char i = 0; i < clippingPlanes.size(); i++) {
		cp = clip(Utils::elementAt(clippingPlanes, i), cp);
		if (cp.size() <= 1)
			break;
	}
	for (auto itt = cp.begin(); itt != cp.end();) {
		if (Utils::isInFront(*itt, ref[0], -refn) <= 0.0f) {
			itt = cp.erase(itt);
		}
		else {
			itt++;
		}
	}
	return cp;
}

Physics::SAT3D::ClippedPoints Physics::SAT3D::clip(Utils::Shapes::Face<glm::vec3, 4>& ref, ClippedPoints inc) const
{
	ClippedPoints res;

	glm::vec3 p1 = inc[0];
	const glm::vec3 n = -ref.getNormal();
	const glm::vec3& refPoint = ref[0];

	for (char i = 1; i <= inc.size(); i++) {
		const glm::vec3& p2 = inc[i % inc.size()];
		const float d1 = Utils::isInFront(p1, refPoint, n);
		const float d2 = Utils::isInFront(p2, refPoint, n);
		glm::vec3 out(0);
		if (d1 > 0 && d2 > 0) {
			res.push_back(p2);
		}
		else if (d1 > 0 && d2 <= 0) {
			out = Utils::linePlaneIntersection(p2 - p1, p1, n, refPoint);
			res.push_back(out);
		}
		else if (d1 <= 0 && d2 > 0) {
			out = Utils::linePlaneIntersection(p2 - p1, p1, n, refPoint);
			res.push_back(out);
			res.push_back(p2);
		}
		p1 = p2;
	}
	return res;
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

	res.points = getContactData(res);
	for (const glm::vec3& p : res.points)
		std::cout << glm::to_string(p) << std::endl;
	std::cout << "----------------------------------------------------------------------------------\n";
	return res;
}
