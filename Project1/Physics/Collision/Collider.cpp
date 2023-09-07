#include "Collider.h"
#include "../../GameObject/GameObject.h"
#include "../Engine.h"
#include <gtc/type_ptr.hpp>


Physics::Collider::Collider() : Component::ComponetBase(), centerOfMass(0), invMass(0), inertiaTensor(1), allVertices(), faces()
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

const float Physics::SphereCollider::getRadius() const
{
	return radius;
}

const glm::vec3 Physics::SphereCollider::support(Vector3 axis, bool local = false) const
{
	glm::vec3 local_p = radius * glm::normalize(axis);
	if (local) {
		return local_p;
	}
	return local_p + getAbsolutePosition();
}



const glm::quat Physics::CubeCollider::getRotation() const
{
	return parent->getGlobalTransform().Rotation;
}

Physics::CubeCollider::CubeCollider() : Collider(), width(0)
{
	allVertices = {
		{  0.5f,  0.5f, 0.5f },
		{ -0.5f,  0.5f, 0.5f },
		{  0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },

		{  0.5f,  0.5f, -0.5f },
		{ -0.5f,  0.5f, -0.5f },
		{  0.5f, -0.5f, -0.5f },
		{ -0.5f, -0.5f, -0.5f }
	};

	faces = {
		Face(Utils::xAxis(), { 0, 2, 4, 6 }),
		Face(Utils::yAxis(), { 0, 1, 4 ,5 }),
		Face(Utils::zAxis(), { 0, 1, 2, 3 }),

		Face(Utils::xAxis(-1), { 1, 3, 5, 7 }),
		Face(Utils::yAxis(-1), { 2, 3, 6, 7 }),
		Face(Utils::zAxis(-1), { 4, 5, 6, 7 }),
	};
}



Physics::CubeCollider::CubeCollider(float width, float mass) : Collider(mass), width(width)
{
	inertiaTensor = glm::mat3((1.f / 12.f) * mass * 4.f * width * width);
}

const glm::vec3 Physics::CubeCollider::support(Vector3 axis, bool local = false) const {
	auto sgn = [](float x) {
		if (x < 0) return -1.f;
		else return 1.f;
	};

	const glm::quat rot_inv = glm::inverse(getRotation());

	const glm::vec3 a = glm::rotate(rot_inv, axis);

	glm::vec3 local_p =  {
		sgn(a.x) * width,
		sgn(a.y) * width,
		sgn(a.z) * width
	};
	if (local) {
		return local_p;
	}

	glm::vec3 world = glm::rotate(getRotation(), local_p);
	world += getAbsolutePosition();
	return world;
}