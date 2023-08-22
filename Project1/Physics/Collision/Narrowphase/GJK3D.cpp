#include "GJK3D.h"
#include "../Collider.h"
#include <tuple>
#include "../../DebugRenderer.h"

bool sameDirection(const glm::vec3& a, const glm::vec3& b) {
	return glm::dot(a, b) > 0;
}

const glm::vec3 Physics::GJK3D::support(const Physics::Collider* a, const Physics::Collider* b, Vector3 dir) const {
	auto n = dir;
	const glm::vec3 v1 = a->support(n);
	const glm::vec3 v2 = b->support(-n);
	return v1 - v2;
}

const Physics::CollisionManfold Physics::GJK3D::getCollisionData(Physics::Collider* a, Physics::Collider* b)
{
#define GJK_MAX_ITTERATION 256
	simplex.reset();
	Physics::CollisionManfold res;

	glm::vec3 axis = Utils::xAxis();

	glm::vec3 A = support(a, b, axis);

	simplex.add(A);
	glm::vec3 D = -A;

	int counter = 0;
	while (counter++ < GJK_MAX_ITTERATION) {
		A = support(a, b, D);
		if (glm::dot(A, D) <= 0.001) {
			// reject
			res.hit = false;
			return res;
		}
		else {
			simplex.add(A);
			// bool contains_origin;
			// tie(s, D, contains_origin) = getNearestSimplex(s);
			if (simplex.evaluate(D)) {
				res.a = a;
				res.b = b;
				res.hit = true;
				std::vector<glm::vec3> polytope(simplex.begin(), simplex.end());
				std::vector<int>  faces = {
					0, 1, 2,
					0, 3, 1,
					0, 2, 3,
					1, 3, 2
				};
				auto [norms, _] = getFaceNormals(polytope, faces);
				std::vector<glm::vec3	> ns;
				for (auto& n : norms) {
					ns.push_back(n);
				}
				DebugRenderer::addSimplex(&simplex, ns);
				EPA(res, simplex);
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
#define GJK_EPA_MAX_ITER 128
	std::vector<glm::vec3> polytope(simplex.begin(), simplex.end());
	std::vector<int>  faces = {
		0, 1, 2,
		0, 3, 1,
		0, 2, 3,
		1, 3, 2
	};

	auto [normals, minFace] = getFaceNormals(polytope, faces);

	glm::vec3 minNormal;
	float minDistance = FLT_MAX;

	int iterations = 0;
	while (minDistance == FLT_MAX)
	{
		minNormal = glm::vec3(normals[minFace]);
		minDistance = normals[minFace].w;

		if (iterations++ > GJK_EPA_MAX_ITER) {
			break;
		}

		glm::vec3 support = this->support(manafold.a, manafold.b, minNormal);
		float sDistance = glm::dot(minNormal, support);

		if (abs(sDistance - minDistance) > 0.001f) {
			minDistance = FLT_MAX;

			std::vector<std::pair<int, int>> uniqueEdges;

			for (int i = 0; i < normals.size(); i++) {
				if (sameDirection(glm::vec3(normals[i]), support)) {
					int f = i * 3;

					addUnique(uniqueEdges, faces, f, f + 1);
					addUnique(uniqueEdges, faces, f + 1, f + 2);
					addUnique(uniqueEdges, faces, f + 2, f);

					faces[f + 2] = faces.back(); faces.pop_back();
					faces[f + 1] = faces.back(); faces.pop_back();
					faces[f] = faces.back(); faces.pop_back();

					normals[i] = normals.back(); normals.pop_back();

					i--;
				}
			}

			if (uniqueEdges.size() == 0) {
				break;
			}

			std::vector<int> newFaces;
			for (auto [edge1, edge2] : uniqueEdges) {
				newFaces.push_back(edge1);
				newFaces.push_back(edge2);
				newFaces.push_back(polytope.size());
			}

			polytope.push_back(support);

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
		manafold.error = true;
		return;
	}

	manafold.normal = minNormal;
	manafold.depth = minDistance + 0.001f;
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
	
		float dist = glm::dot(norm, -abs(v1));

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
		line(dir);
		break;
	case 3:
		triangle(dir);
		break;
	case 4:
		return prism(dir);
	}
	return false;
}

void Physics::Simplex::reset()
{
	data_ = {};
	size_ = 0;
}

void Physics::Simplex::line(glm::vec3& dir)
{
	const glm::vec3 ao = -data_[0];
	const glm::vec3 ab = data_[1] + ao;

	if (sameDirection(ao, ab)) {
		dir = glm::cross(ab, ao);
		dir = glm::cross(dir, ab);
	}
	else {
		data_ = { data_[0] };
		dir = ao;
	}
}

void Physics::Simplex::triangle(glm::vec3& dir)
{
	const glm::vec3 a = data_[0];
	const glm::vec3 b = data_[1];
	const glm::vec3 c = data_[2];

	const glm::vec3 ab = b - a;
	const glm::vec3 ac = c - a;
	const glm::vec3 ao = -a;

	const glm::vec3 abc = glm::cross(ab, ac);

	if (sameDirection(glm::cross(abc, ac), ao)) {
		if (sameDirection(ac, ao)) {
			data_ = { a, c };
			dir = glm::cross(ac, ao);
			dir = glm::cross(dir, ac);
		}
		else {
			line(dir);
			data_ = { a, b, c };
		}
	}
	else {
		if (sameDirection(glm::cross(ab, abc), ao)) {
			line(dir);
			data_ = { a, b, c };
		}
		else {
			data_ = { a, c, b };
			dir = -abc;
		}
	}
}

bool Physics::Simplex::prism(glm::vec3& dir)
{
	const glm::vec3 a = data_[0];
	const glm::vec3 b = data_[1];
	const glm::vec3 c = data_[2];
	const glm::vec3 d = data_[3];

	const glm::vec3 ab = b - a;
	const glm::vec3 ac = c - a;
	const glm::vec3 ad = d - a;
	const glm::vec3 ao = -a;

	const glm::vec3 abc = glm::cross(ab, ac);
	const glm::vec3 acd = glm::cross(ac, ad);
	const glm::vec3 adb = glm::cross(ad, ab);

	if (sameDirection(abc, ao)) {
		triangle(dir);
		return false;
	}

	if (sameDirection(acd, ao)) {
		data_ = { a, c, d };
		triangle(dir);
		data_ = { a, b, c, d };
		return false;
	}

	if (sameDirection(adb, ao)) {
		data_ = { a, d, b };
		triangle(dir);
		data_ = { a, b, c, d };
		return false;
	}

	return true;
}

bool Physics::Simplex::sameDirection(Vector3 a, Vector3 b) const
{
	return glm::dot(a, b) > 0;
}
