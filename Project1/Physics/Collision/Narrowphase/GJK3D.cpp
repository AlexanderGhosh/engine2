#include "GJK3D.h"
#include "../Collider.h"
#include <tuple>

bool sameDirection(const glm::vec3& a, const glm::vec3& b) {
	return glm::dot(a, b) > 0;
}

Physics::SupportPoint support(const Physics::Collider* a, const Physics::Collider* b, const glm::vec3& dir) {
	Physics::SupportPoint res;
	auto n = glm::normalize(dir);
	res.a = a->support(n);
	res.b = b->support(-n);
	res.v = res.a - res.b;
	res.dir = dir;
	////////////////////////////////////////////
	// res.a = a->support(-dir);
	// res.b = b->support(dir);
	// res.v = res.b - res.a;
	return res;
}

const Physics::CollisionManfold Physics::GJK3D::getCollisionData(Physics::Collider* a, Physics::Collider* b)
{
#define GJK_MAX_ITTERATION 256

	Physics::CollisionManfold res;

	glm::vec3 axis = Utils::xAxis();

	glm::vec3 A = support(a, b, axis).v;

	Simplex s;
	s.add(A);
	glm::vec3 D = -A;

	int counter = 0;
	while (counter++ < GJK_MAX_ITTERATION) {
		A = support(a, b, D).v;
		if (glm::dot(A, D) <= 0.001) {
			// reject
			res.hit = false;
			return res;
		}
		else {
			s.add(A);
			// bool contains_origin;
			// tie(s, D, contains_origin) = getNearestSimplex(s);
			if (s.evaluate(D)) {
				res.a = a;
				res.b = b;
				res.hit = true;
				EPA(res, s);
				return res;
			}
		}
	}

	return {};
}

void addUnique(std::vector<std::pair<int, int>>& edges, const std::vector<int>& faces, int a, int b) {
	auto reverse = std::find(             
		edges.begin(),                    
		edges.end(),                      
		std::make_pair(faces[b], faces[a])
	);

	if (reverse != edges.end()) {
		edges.erase(reverse);
	}

	else {
		edges.emplace_back(faces[a], faces[b]);
	}
}

void Physics::GJK3D::EPA(Physics::CollisionManfold& manafold, const Physics::Simplex& prism) const
{
	std::vector<glm::vec3> polytope(prism.begin(), prism.end());
	std::vector<int> faces = {
		0, 1, 2,
		0, 3, 1,
		0, 2, 3,
		1, 3, 2
	};

	auto [normals, minFace] = getFaceNormals(polytope, faces);

	glm::vec3 minNormal;
	float minDistance = INFINITY;

	int itt = 0;
	while (minDistance == INFINITY && itt++ < GJK_MAX_ITTERATION) {
		Vector4 n = normals[minFace];
		minNormal = glm::vec3(n);
		minDistance = n.z;

		glm::vec3 support_ = support(manafold.a, manafold.b, minNormal).v;
		float sDistance = glm::dot(minNormal, support_);

		if (abs(sDistance - minDistance) > 0) {
			// not found
			minDistance = INFINITY;

			std::vector<std::pair<int, int>> uniqueEdges;

			for (int i = 0; i < normals.size(); i++) {
				if (sameDirection(normals[i], minNormal)) {
					int f = i * 3;

					addUnique(uniqueEdges, faces, f, f + 1);
					addUnique(uniqueEdges, faces, f + 1, f + 2);
					addUnique(uniqueEdges, faces, f + 2, f);

					faces[f + 2] = faces.back();
					faces.pop_back();

					faces[f + 1] = faces.back();
					faces.pop_back();

					faces[f] = faces.back();
					faces.pop_back();

					normals[i] = normals.back();
					normals.pop_back();

					i--;
				}
			}
			if (uniqueEdges.size() == 0) {
				break;
			}

			std::vector<int> newFaces;
			for (auto [e1, e2] : uniqueEdges) {
				newFaces.push_back(e1);
				newFaces.push_back(e2);
				newFaces.push_back(polytope.size());
			}

			polytope.push_back(support_);


			auto [newNormals, newMinFace] = getFaceNormals(polytope, newFaces);

			float newMinDistance = FLT_MAX;
			for (int i = 0; i < normals.size(); i++) {
				if (normals[i].w < newMinDistance) {
					newMinDistance = normals[i].w;
					minFace = i;
				}
			}

			if (newNormals[newMinFace].w < newMinDistance) {
				minFace = newMinFace + normals.size();
			}

			faces.insert(faces.end(), newFaces.begin(), newFaces.end());
			normals.insert(normals.end(), newNormals.begin(), newNormals.end());
		}
	}

	if (minDistance == FLT_MAX) {
		manafold.hit = false;
		manafold.error = true;
	}
	else {
		manafold.hit = true;
		manafold.error = false;
		manafold.normal = minNormal;
		manafold.depth = minDistance + 0.001f;
	}
}

std::pair<std::vector<glm::vec4>, int> Physics::GJK3D::getFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<int>& faces) const
{
	std::vector<glm::vec4> normals;
	normals.reserve(faces.size() / 3);

	int minTriangle = 0;
	float minDistance = INFINITY;

	for (int i = 0; i < faces.size(); i += 3) {
		Vector3 v1 = polytope[faces[i]];
		Vector3 v2 = polytope[faces[i + 1]];
		Vector3 v3 = polytope[faces[i + 2]];

		glm::vec3 norm = glm::cross((v2 - v1), (v3 - v1));
		norm = glm::normalize(norm);
	
		float dist = glm::dot(norm, v1);

		if (dist < 0) {
			norm *= -1;
			dist *= -1;
		}
		normals.emplace_back(norm, dist);

		if (dist < minDistance) {
			minDistance = dist;
			minTriangle = i / 3;
		}

	}
	return { normals, minTriangle };
}


Physics::Simplex::Simplex() : data_(), size_(0)
{
}

Physics::Simplex::Simplex(std::initializer_list<glm::vec3> list) : Simplex()
{
	for (auto v = list.begin(); v != list.end(); v++) {
		data_[std::distance(list.begin(), v)] = *v;
	}
	size_ = list.size();
}

void Physics::Simplex::add(Vector3 v)
{
	data_ = { v, data_[0], data_[1], data_[2]};
	size_ = std::min(size_ + 1, 4u);
}

bool Physics::Simplex::evaluate(glm::vec3& dir)
{
	switch (size_)
	{
	case 2:
		return line(data_, dir);
	case 3:
		return triangle(data_, dir);
	case 4:
		return prism(data_, dir);
	}
	return false;
}

void Physics::Simplex::line(glm::vec3& dir)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];

	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;

	if (sameDirection(ab, ao)) {
		direction = glm::cross(glm::cross(ab, ao), ab);
	}

	else {
		points = { a };
		direction = ao;
	}
	return false;
}

bool Physics::Simplex::triangle(std::array<glm::vec3, 4>& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = cross(ab, ac);

	if (sameDirection(cross(abc, ac), ao)) {
		if (sameDirection(ac, ao)) {
			points = { a, c };
			direction = glm::cross(glm::cross(ac, ao), ac);
		}

		else {
			return line(points = { a, b }, direction);
		}
	}

	else {
		if (sameDirection(glm::cross(ab, abc), ao)) {
			return line(points = { a, b }, direction);
		}

		else {
			if (sameDirection(abc, ao)) {
				direction = abc;
			}

			else {
				points = { a, c, b };
				direction = -abc;
			}
		}
	}
	return false;
}

bool Physics::Simplex::prism(std::array<glm::vec3, 4>& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];
	glm::vec3 d = points[3];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ad = d - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = glm::cross(ab, ac);
	glm::vec3 acd = glm::cross(ac, ad);
	glm::vec3 adb = glm::cross(ad, ab);

	if (sameDirection(abc, ao)) return triangle(points = { a, b, c }, direction);
	if (sameDirection(acd, ao)) return triangle(points = { a, c, d }, direction);
	if (sameDirection(adb, ao)) return triangle(points = { a, d, b }, direction);

	return true;
}

bool Physics::Simplex::sameDirection(Vector3 a, Vector3 b)
{
	return glm::dot(a, b) > 0;
}
