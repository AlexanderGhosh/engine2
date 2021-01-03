#include "Collider.h"
#include "../Engine.h"
#include <gtx/string_cast.hpp>
#include "../../GameObject/GameObject.h"

glm::vec3 Physics::AABB::support(const glm::vec3& direction) const
{
	glm::vec3 res = *position;
	for (char i = 0; i < 3; i++)
		res[i] += direction[i] < 0 ? min[i] : max[i];
	return res;
}

void Physics::AABB::createInertiaTensor()
{
	const float m = mass * 0.5f;
	const float w = pow(scale->x, 2.0f);
	const float h = pow(scale->y, 2.0f);
	const float d = pow(scale->z, 2.0f);
	localInertiaTensor = {
		{ m * (h + d), 0, 0 },
		{ 0, m * (w + d), 0 },
		{ 0, 0, m * (w + h) }
	};
}

void Physics::Collider::setParent(GameObject* parent)
{
	Base::setParent(parent);
	Component::Transform* t = parent->getTransform();
	position = &t->Position;
	rotation = &t->Rotation;
	scale = &t->Scale;
}

Physics::Collider::Collider(bool add) : mass(10), localCentroid(0), localInertiaTensor(0), position(nullptr), scale(nullptr), rotation(nullptr) {
	if(add)
		Engine::addCollider(this);
}

const Physics::AABB* Physics::BoxCollider::constructAABB()
{
	std::array<glm::vec3, 8> translated = {
		min, max,
		{min.x, max.y, max.z},
		{min.x, min.y, max.z},
		{min.x, max.y, min.z},

		{max.x, min.y, min.z},
		{max.x, max.y, min.z},
		{max.x, min.y, max.z},
	};
	glm::vec3 min(INFINITY);
	glm::vec3 max(-INFINITY);
	for (glm::vec3& tra : translated) {
		tra *= *scale;
		tra = glm::rotate(*rotation, tra);
		tra += *position;
		for (char i = 0; i < 3; i++) {
			if (tra[i] < min[i]) {
				min[i] = tra[i];
			}
			if (tra[i] > max[i]) {
				max[i] = tra[i];
			}
		}
	}
	aabb.setMin(min - *position);
	aabb.setMax(max - *position);
	aabb.setParent(this->getParent());
	return &aabb;
}

glm::vec3 Physics::BoxCollider::support(const glm::vec3& direction) const
{
	glm::vec3 res(0);
	const glm::vec3 dir = direction * glm::inverse(glm::toMat3(*rotation));

	res = glm::sign(dir) * *scale * 0.5f;
	for (char i = 0; i < 3; i++) {
		res[i] = dir[i] > 0 ? max[i] : min[i];
	}

	res = (res * glm::toMat3(*rotation)) + *position;
	// std::cout << glm::to_string(res) << "/#/\n";
	return res;
}

const Physics::AABB* Physics::BoxColliderSAT::constructAABB()
{
	glm::vec3 mi = Utils::fill(-0.5f), ma = Utils::fill(0.5f);
	std::array<glm::vec3, 8> translated = {
		mi, ma,
		{mi.x, ma.y, ma.z},
		{mi.x, mi.y, ma.z},
		{mi.x, ma.y, mi.z},

		{ma.x, mi.y, mi.z},
		{ma.x, ma.y, mi.z},
		{ma.x, mi.y, ma.z},
	};
	glm::vec3 min(INFINITY);
	glm::vec3 max(-INFINITY);
	for (glm::vec3& tra : translated) {
		tra *= *scale;
		tra = glm::rotate(*rotation, tra);
		tra += *position;
		for (char i = 0; i < 3; i++) {
			if (tra[i] < min[i]) {
				min[i] = tra[i];
			}
			if (tra[i] > max[i]) {
				max[i] = tra[i];
			}
		}
	}
	return DBG_NEW AABB(min - *position, max - *position, false, parent);
}

std::list<glm::vec3> Physics::ColliderSAT::getAllFaces(const glm::vec3& vertex, bool localSpace, bool getIndices) const
{
	std::list<glm::vec3> res;
	auto transform = [&](glm::vec3 v)->glm::vec3 {
		if (localSpace)
			return v;
		return v * *scale * glm::toMat3(*rotation) + *position;
	};
	for (unsigned i = 0; i < indices.size(); i++) {
		const unsigned& index = indices[i];
		if (vertices[index] == vertex) {
			unsigned j = i % faceSize;
			j = i - j;
			for (char k = 0; k < faceSize; k++) {
				if (!getIndices)
					res.push_back(transform(vertices[indices[j + k]]));
				else
					res.push_back(Utils::fill(j + k));
			}
		}
	}
	return res;
}

std::list<glm::vec3> Physics::ColliderSAT::getAllFaces(const unsigned& vertexIndex, bool localSpace, bool getIndices) const
{
	return getAllFaces(vertices[vertexIndex], localSpace, getIndices);
}

std::list<std::list<glm::vec3>> Physics::ColliderSAT::getAdjacentFaces(std::list<glm::vec3>& face) const
{
	std::vector<char> facesAdded;
	std::list<std::list<glm::vec3>> res;
	for (const glm::vec3& v : face) {
		for (unsigned i = 0; i < indices.size(); i++) {
			const glm::vec3 vert = vertices[indices[i]] * *scale * *rotation + *position;
			const char faceIndex = static_cast<int>(floor(i / 4));
			if (vert == v AND NOT Utils::contains(facesAdded, faceIndex)) {
				facesAdded.push_back(faceIndex);
				std::list<glm::vec3> f;
				for (int j = 0; j < 4; j++) {
					f.push_back(vertices[indices[faceIndex * faceSize + j]] * *scale * *rotation + *position);
				}
				if (f != face) {
					res.push_back(f);
					break;
				}
				else
					f.clear();
			}
			else if (vert == v) {
				int t = 0;
			}
		}
	}
	return res;
}
