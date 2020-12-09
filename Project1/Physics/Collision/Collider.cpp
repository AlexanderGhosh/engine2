#include "Collider.h"
#include "../Engine.h"
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
	glm::vec3 dir = glm::inverse(glm::toMat3(*rotation)) * direction;
	for (char i = 0; i < 3; i++)
		res[i] = dir[i] > 0 ? max[i] : min[i];
	return glm::toMat3(*rotation) * res + *position;;
}
