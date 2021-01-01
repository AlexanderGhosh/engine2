#include "Rigidbody2.h"
#include "../Physics/Engine.h"
#include "../GameObject/GameObject.h"
#include "../Physics/Collision/Collider.h"

Component::Rigidbody2::Rigidbody2()
{
	// Physics::Engine::addRigidBody(this);
}

void Component::Rigidbody2::addCollider(Physics::Collider* collider)
{
	colliders.push_back(collider);
	mass += collider->mass;
	invMass = 1.0f / mass;

	// do stuff for the inertia tensor
}

void Component::Rigidbody2::update()
{
	intergrateForces();
	intergrateVelocity();
	clearForces();
}

void Component::Rigidbody2::setParent(GameObject* parent)
{
	Base::setParent(parent);
	transform = parent->getTransform();
	rotation = &transform->Rotation;
}

void Component::Rigidbody2::addForce(Vector3 force, Vector3 at)
{
	this->force += force;
	this->torque += glm::cross(at - cos, force);
}

void Component::Rigidbody2::intergrateForces()
{
	const float& dt = Physics::Engine::getDeltaTime();
	if (invMass)
		velocity += Physics::Engine::getGravity() * dt;

	velocity += force * invMass * dt;

	velocity *= Physics::Engine::getDamppingFactor();

	angularVelocity += g_invInertia * torque * dt;

	angularVelocity *= Physics::Engine::getDamppingFactor();
}

void Component::Rigidbody2::intergrateVelocity()
{	
	const float& dt = Physics::Engine::getDeltaTime();
	transform->Position += velocity * dt;
	cos += velocity * dt;

	const glm::quat deltaQ(0.0f, angularVelocity * dt * 0.5f);
	*rotation = *rotation * deltaQ * *rotation;
	*rotation = glm::normalize(*rotation);

	g_invInertia = g_invInertia * glm::toMat3(deltaQ * *rotation);
}

void Component::Rigidbody2::clearForces()
{
	force = Utils::zero();
	torque = Utils::zero();
}
