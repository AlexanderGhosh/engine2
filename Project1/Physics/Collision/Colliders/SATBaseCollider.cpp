#include "SATBaseCollider.h"
#include "../../../GameObject/GameObject.h"
#include "../../Engine.h"

SATBaseCollider::SATBaseCollider() : vertices(), normals(), invMass(0), inertiaTensor(1)
{
	Physics::Engine::addCollider(this);
}

const glm::mat3 SATBaseCollider::getGlobalInvInertiaTensor() const
{
	glm::quat r = parent->getGlobalTransform().Rotation;
	glm::mat3 rot = glm::toMat3(r);
	glm::mat3 global = rot * inertiaTensor * glm::transpose(rot);
	return glm::inverse(global);
}

void SATBaseCollider::setParent(GameObject* obj)
{
	Component::ComponetBase::setParent(obj);
	transform = parent->getLocalTransform();
}

void SATBaseCollider::cleanUp()
{
}

std::array<float, 2> SATBaseCollider::project(Vector3 axis) const
{
	std::array<float, 2> res = { INFINITY, -INFINITY };

	float& min = res[0];
	float& max = res[1];

	for (Vector3 v : getGlobalVertices()) {
		float a = glm::dot(v, axis);
		min = fminf(min, a);
		max = fmaxf(max, a);
	}
	return res;
}
