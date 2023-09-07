#include "GJK3D.h"
#include "../Collider.h"
#include <tuple>
#include "../../DebugRenderer.h"
#include "../../../GameObject/GameObject.h"

const glm::vec3 getBarycentricCoords(Vector3 p, Vector3 a, Vector3 b, Vector3 c);

bool sameDirection(const glm::vec3& a, const glm::vec3& b) {
	return glm::dot(a, b) > 0;
}

const glm::vec3 Physics::GJK3D::support(const Physics::Collider* a, const Physics::Collider* b, Vector3 dir) const {
	auto n = dir;
	const glm::vec3 v1 = a->support(n);
	const glm::vec3 v2 = b->support(-n);
	return v1 - v2;
}

const Physics::CollisionDetails Physics::GJK3D::getCollisionData(Physics::Collider* a, Physics::Collider* b)
{
#define GJK_MAX_ITTERATION 1024
	simplex.reset();
	Physics::CollisionDetails res;
	res.a = a;
	res.b = b;

	glm::vec3 axis(1.f);

	glm::vec3 A = support(a, b, axis).v;

	simplex.add(A, axis);
	glm::vec3 D = -A;

	int counter = 0;
	while (counter++ < GJK_MAX_ITTERATION) {
		A = support(a, b, D);
		auto t = glm::dot(A, D);
		if (glm::dot(A, D) <= 0) {
			// reject
			res.hit = false;
			return res;
		}
		else {
			simplex.add(A, D);
			// bool contains_origin;
			// tie(s, D, contains_origin) = getNearestSimplex(s);
			if (s.evaluate(D)) {
				res.a = a;
				res.b = b;
				res.hit = true;

				// simplex.add({ 0 , 1, 0 });
				// simplex.add({ -.5 , 0, .5 });
				// simplex.add({ .5 , 0, .5 });
				// simplex.add({ 0 , 0, -.5 });

				std::vector<glm::vec3> polytope(simplex.begin_data(), simplex.end_data());
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

void Physics::GJK3D::EPA(Physics::CollisionDetails& details, const Physics::Simplex& prism) const
{
#define GJK_EPA_MAX_ITER 128
	std::vector <glm::vec3> polytope(simplex.begin_data(), simplex.end_data());
	std::vector<glm::vec3> directions(simplex.begin_dir(), simplex.end_dir());

	std::vector<glm::vec3> supportDirections;
	std::vector<int>  faces = {
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

		glm::vec3 support = this->support(details.a, details.b, minNormal);
		float sDistance = glm::dot(minNormal, support);

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

			polytope.push_back(support);
			directions.push_back(minNormal);

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
		else {
		}
	}

	if (minDistance == FLT_MAX) {
		details.error = true;
		return;
	}

	details.intersection_norm = minNormal;
	details.intersection_distance = minDistance + 0.001f;

	const int face_index = minFace * 3;
	const glm::vec3 v1 = polytope[faces[face_index]];
	const glm::vec3 v2 = polytope[faces[face_index + 1]];
	const glm::vec3 v3 = polytope[faces[face_index + 2]];

	const glm::vec3 d1 = directions[faces[face_index]];
	const glm::vec3 d2 = directions[faces[face_index + 1]];
	const glm::vec3 d3 = directions[faces[face_index + 2]];

	calculateContactPoint(details, v1, v2, v3, d1, d2, d3);
}

void Physics::GJK3D::calculateContactPoint(CollisionDetails& details, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 dir1, Vector3 dir2, Vector3 dir3) const
{
	float t = glm::dot(v1, details.intersection_norm);
	// t /= glm::dot(details.intersection_norm, details.intersection_norm);

	glm::vec3 cloest_point = t * details.intersection_norm;

	glm::vec3 bary = getBarycentricCoords(cloest_point, v1, v2, v3);

	std::array<glm::vec3, 3> supports_a, supports_b;

	supports_a[0] = details.a->support(dir1, true);
	supports_a[1] = details.a->support(dir2, true);
	supports_a[2] = details.a->support(dir3, true);

	supports_b[0] = details.b->support(-dir1, true);
	supports_b[1] = details.b->support(-dir2, true);
	supports_b[2] = details.b->support(-dir3, true);

	glm::vec3 a_local = bary.x * supports_a[0] + bary.y * supports_a[1] + bary.z * supports_a[2];
	glm::vec3 b_local = bary.x * supports_b[0] + bary.y * supports_b[1] + bary.z * supports_b[2];

	// Component::Transform* a_t = details.a->getParent()->getLocalTransform();
	// Component::Transform* b_t = details.a->getParent()->getLocalTransform();

	// a_local -= details.a->getAbsolutePosition();
	// a_local = glm::rotate(glm::inverse(a_t->Rotation), a_local);
	//   
	// b_local -= details.b->getAbsolutePosition();
	// b_local = glm::rotate(glm::inverse(b_t->Rotation), b_local);

	details.a_hit = a_local;
	details.b_hit = b_local;
}

const glm::vec3 getBarycentricCoords(Vector3 p, Vector3 a, Vector3 b, Vector3 c) {
	glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	glm::vec3 res;
	res.x = (d11 * d20 - d01 * d21) / denom;
	res.y = (d00 * d21 - d01 * d20) / denom;
	res.z = 1.0f - res.x - res.y;
	return res;
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


Physics::Simplex::Simplex() : data_(), size_(0), directions_()
{
}

Physics::Simplex::Simplex(std::initializer_list<glm::vec3> list) : Simplex()
{
	for (auto v = list.begin(); v != list.end(); v++) {
		data_[std::distance(list.begin(), v)] = *v;
	}
	size_ = list.size();
}

void Physics::Simplex::add(Vector3 v, Vector3 dir)
{
	data_ = { v, data_[0], data_[1], data_[2]};
	size_ = std::min(size_ + 1, 4u);
	directions_ = { v, directions_[0], directions_[1], directions_[2] };
}

void Physics::Simplex::reorder(std::initializer_list<int> order) {
	std::array<glm::vec3, 4> newData;
	for (auto i = order.begin(); i != order.end(); i++) {
		newData[std::distance(order.begin(), i)] = data_[*i];
	}
	std::array<glm::vec3, 4> newDirs;
	for (auto i = order.begin(); i != order.end(); i++) {
		newDirs[std::distance(order.begin(), i)] = directions_[*i];
	}
	size_ = order.size();
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

void Physics::Simplex::reset()
{
	data_ = {};
	directions_ = {};
	size_ = 0;
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
		reorder({ 0 });
		// data_ = { data_[0] };
		// size_ = 1;
		dir = ao;
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
			// data_ = { a, c };
			// size_ = 2;
			reorder({ 0, 2 });
			dir = glm::cross(ac, ao);
			dir = glm::cross(dir, ac);
		}

		else {
			reorder({ 0, 1 });
			// data_ = { a, b };
			// size_ = 2;
			line(dir);
		}
	}

	else {
		if (sameDirection(glm::cross(ab, abc), ao)) {
			reorder({ 0, 1 });
			// data_ = { a, b };
			// size_ = 2;
			line(dir);
		}

		else {
			if (sameDirection(abc, ao)) {
				dir = abc;
			}
			else {
				reorder({ 0, 2, 1 });
				// data_ = { a, c, b };
				dir = -abc;
			}
		}
	}
	return false;
}

bool Physics::Simplex::prism(std::array<glm::vec3, 4>& points, glm::vec3& direction)
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
		// data_ = { a, b, c };
		// size_ = 3;
		reorder({ 0, 1, 2 });
		triangle(dir);
		return false;
	}

	if (sameDirection(acd, ao)) {
		// data_ = { a, c, d };
		// size_ = 3;
		reorder({ 0, 2, 3 });
		triangle(dir);
		return false;
	}

	if (sameDirection(adb, ao)) {
		// data_ = { a, d, b };
		// size_ = 3;
		reorder({ 0, 3, 1 });
		triangle(dir);
		return false;
	}

	return true;
}

bool Physics::Simplex::sameDirection(Vector3 a, Vector3 b)
{
	return glm::dot(a, b) > 0;
}
