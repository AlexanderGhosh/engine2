#include "Collider.h"
#include "../../GameObject/GameObject.h"
#include "../Engine.h"
#include <gtc/type_ptr.hpp>



Physics::SphereCollider::SphereCollider() : radius(0), centerOfMass(0), invMass(0), inertiaTensor(1)
{
}

Physics::SphereCollider::SphereCollider(float radius, float mass) : SphereCollider()
{
	this->radius = radius;
	Physics::Engine::addCollider(this);

	inertiaTensor = glm::mat3(.4f * mass * radius * radius);
	this->invMass = 1.f / mass;
}

const glm::vec3 Physics::SphereCollider::getAbsolutePosition() const
{
	return parent->getGlobalTransform().Position + centerOfMass;
}

const glm::mat3 Physics::SphereCollider::getGlobalInvInertiaTensor() const
{
	glm::quat r = parent->getGlobalTransform().Rotation;
	glm::mat3 rot = glm::toMat3(r);
	glm::mat3 global = rot * inertiaTensor * glm::transpose(rot);
	return glm::inverse(global);
}

void Physics::SphereCollider::setParent(GameObject* parent)
{
	Component::ComponetBase::setParent(parent);
}

void Physics::SphereCollider::cleanUp() {

}
