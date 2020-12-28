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
	glm::vec3 v = coll->getFarthest(n, index);
	// index = coll->rawToIndices(index);
	std::list<glm::vec3> faces = coll->getAllFaces(index, false, true);
	glm::vec3 best = Utils::zero();
	SHAPE::Face<glm::vec3, 4> face;
	float val = 0;
	for (auto itt = faces.begin(); itt != faces.end(); std::advance(itt, 4)) {
		glm::vec3 norm = coll->getNormal((*itt).x, true);
		//glm::vec3 norm = Utils::yAxis();
		float t = fabsf(glm::dot(glm::normalize(n), glm::normalize(norm)));
		if (t > val) {
			val = t;
			best = norm;
			face.set({ *(itt++), *(itt++), *(itt++), *(itt++) });
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
	n0 = Utils::yAxis();
	glm::vec3 n1 = f2.getNormal();
	if (fabsf(glm::dot(n0, n)) > fabsf(glm::dot(n1, n))) {
		ref = f1;
		inc = f2;
		r = a;
		i = b;
		fliped = false;
	}

	// begining the clipping

	const glm::vec3 refn = glm::normalize(n0);

	float o1 = glm::dot(refn, ref[0]);
	std::list <Face<glm::vec3, 4>> clippingPlanes;
	std::list<std::list<glm::vec3>> adjacent = r->getAdjacentFaces(ref.getDataList());
	for (const std::list<glm::vec3>& adj : adjacent)
		clippingPlanes.push_back(Face<glm::vec3, 4>(adj));
	
	ClippedPoints cp = inc.getDataVector();
	for (Face<glm::vec3, 4>& plane : clippingPlanes) {
		cp = clip(plane, cp);
		if (cp.size() == 1)
			break;
	}
	return cp;
}

Physics::SAT3D::ClippedPoints Physics::SAT3D::clip(Utils::Shapes::Face<glm::vec3, 4>& ref, const ClippedPoints& inc) const
{
	ClippedPoints res;

	glm::vec3 point2 = inc[inc.size() - 1];
	const glm::vec3 n = ref.getNormal();
	const glm::vec3& refPoint = ref[0];

	for (char i = 0; i < inc.size(); i++) {
		glm::vec3 out(0);
		const glm::vec3& point1 = inc[i];
		if (Utils::isInFront(point1, refPoint, n) && Utils::isInFront(point2, refPoint, n)) {
			res.push_back(point2);
		}
		else if (Utils::isInFront(point1, refPoint, n) && !Utils::isInFront(point2, refPoint, n)) {
			out = Utils::linePlaneIntersection(point2 - point1, point2, n, refPoint);
			res.push_back(out);
		}
		else if(!Utils::isInFront(point1, refPoint, n) && Utils::isInFront(point2, refPoint, n)) {
			out = Utils::linePlaneIntersection(point2 - point1, point2, n, refPoint);
			res.push_back(point1);
			res.push_back(out);
		}
		/*else if (!Utils::isInFront(point1, ref[0], n) && !Utils::isInFront(point2, ref[0], n)) {
			glm::vec3 out(0);
			Utils::linePlaneIntersection(point1, ref, out);
			res.push_back(out);
			res.push_back(point1);
		}*/
		point2 = point1;
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
