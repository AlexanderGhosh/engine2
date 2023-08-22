#include "Collider.h"
#include "../../GameObject/GameObject.h"
#include "../Engine.h"
#include <gtc/type_ptr.hpp>


Physics::Collider::Collider() : Component::ComponetBase(), centerOfMass(0), invMass(0), inertiaTensor(1)
{
	Physics::Engine::addCollider(this);
}

Physics::Collider::Collider(float mass) : Collider()
{
	invMass = 1.f / mass;
}

Physics::SphereCollider::SphereCollider() : Physics::Collider(), radius(0)
{
}

Physics::SphereCollider::SphereCollider(float radius, float mass) : Physics::Collider(mass), radius(radius)
{
	inertiaTensor = glm::mat3(.4f * mass * radius * radius);
}

const glm::vec3 Physics::Collider::getAbsolutePosition() const
{
	return parent->getGlobalTransform().Position + centerOfMass;
}

void Physics::Collider::cleanUp()
{
}

const glm::mat3 Physics::Collider::getGlobalInvInertiaTensor() const
{
	glm::quat r = parent->getGlobalTransform().Rotation;
	glm::mat3 rot = glm::toMat3(r);
	glm::mat3 global = rot * inertiaTensor * glm::transpose(rot);
	return glm::inverse(global);
}

const float Physics::SphereCollider::getRadius(float theta) const
{
	return radius;
}

const glm::vec3 Physics::SphereCollider::support(Vector3 axis) const
{
	return (radius * glm::normalize(axis)) + getAbsolutePosition();
}



/*Physics::CubeCollider::CubeCollider() : Collider(), width(0)
{
}



Physics::CubeCollider::CubeCollider(float width, float mass) : Collider(mass), width(width)
{
	inertiaTensor = glm::mat3((1.f / 12.f) * mass * 2.f * width * width);
}



const float Physics::CubeCollider::getRadius(float theta) const
{
	while (theta >= 360) {
		theta -= 360;
	}
	theta = RADIANS(theta);
	if (theta == 0) {
		return width;
	}
	float d = sinf(theta);
	return width / d;
}
*/