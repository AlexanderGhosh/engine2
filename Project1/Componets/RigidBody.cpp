#include "Rigidbody.h"
#include "../Physics/Engine.h"
#include "../GameObject/GameObject.h"
#include "../Physics/Collision/Collider.h"

Component::RigidBody::RigidBody() : ComponetBase(), transform(nullptr), cos(0), rotation(0), velocity(0), angularVelocity(0), isKinimatic(false),
force(0), torque(0), mass(0), invMass(0), l_inertia(1), l_invInertia(1), g_inertia(1), g_invInertia(1), colliders(), hasGravity(true)
{
	invMass = 1.0f / mass;
	l_invInertia *= invMass;
	g_invInertia *= invMass;
	Physics::Engine::addRigidBody(this);
}

void Component::RigidBody::addCollider(Physics::Collider* collider)
{
	colliders.push_back(collider);
	mass += collider->mass;
	invMass = 1.0f / mass;

	l_inertia = collider->localInertiaTensor;
	l_invInertia = glm::inverse(l_inertia);
	updateInertia();
}

void Component::RigidBody::updateInertia()
{
	auto g = getRotationMatrix();
	auto t = Utils::inverse(g);
	g_inertia = l_inertia;
	g_invInertia = Utils::inverse(g_inertia);
}

void Component::RigidBody::setParent(GameObject* parent)
{
	ComponetBase::setParent(parent);
	transform = parent->getTransform();
	rotation = &transform->Rotation;
	updateInertia();
}

void Component::RigidBody::cleanUp()
{
	parent = nullptr;

	rotation = nullptr;

	transform = nullptr;

	for (auto itt = colliders.begin(); itt != colliders.end();) {
		itt = colliders.erase(itt);
	}

}

void Component::RigidBody::addForce(Vector3 force, Vector3 at)
{
	this->force += force;
	this->torque += glm::cross(at - cos, force);
}

Vector3 Component::RigidBody::getCOM() const
{
	return transform->Position;
}

Quaternion Component::RigidBody::getRotation() const
{
	return *rotation;
}

glm::mat3 Component::RigidBody::getRotationMatrix() const
{
	return glm::mat3_cast(*rotation);
}

Vector3 Component::RigidBody::getVelocity() const
{
	return velocity;
}

Vector3 Component::RigidBody::getAngularVelocity() const
{
	return angularVelocity;
}

Matrix3 Component::RigidBody::getInvInertia_G() const
{
	return g_invInertia;
}

const float& Component::RigidBody::getInvMass() const
{
	return invMass;
}
const float& Component::RigidBody::getMass() const
{
	return 1.0f / invMass;
}

void Component::RigidBody::velocityAdder(Vector3 add)
{
	velocity += Utils::round(add, Physics::Engine::roundResolution);
}

void Component::RigidBody::angularVelAdder(Vector3 add)
{
	angularVelocity += add;
}

void Component::RigidBody::cosAdder(Vector3 add)
{
	transform->Position += add;
	cos += add;
}

void Component::RigidBody::intergrateForces()
{
	if (isKinimatic) {
		clearForces();
		return;
	}
	const float& dt = Physics::Engine::getDeltaTime();
	if (invMass AND hasGravity)
		velocity += Physics::Engine::getGravity() * dt;

	velocity += force * invMass * dt;

	velocity *= Physics::Engine::getDamppingFactor();
	velocity = Utils::round(velocity, Physics::Engine::roundResolution);

	angularVelocity += g_invInertia * torque * dt;

	angularVelocity *= Physics::Engine::getDamppingFactor();
	clearForces();
}

void Component::RigidBody::intergrateVelocity()
{	
	if (isKinimatic)
		return;
	const float& dt = Physics::Engine::getDeltaTime();
	velocity = Utils::round(velocity, Physics::Engine::roundResolution);

	transform->Position += velocity * dt;
	cos += velocity * dt;
	return;
	const glm::quat deltaQ(0.0f, angularVelocity * dt * 0.5f);
	*rotation += deltaQ * *rotation;
	*rotation = glm::normalize(*rotation);
}

void Component::RigidBody::clearForces()
{
	force = Utils::zero();
	torque = Utils::zero();
	if (isKinimatic) {
		velocity = Utils::zero();
		angularVelocity = Utils::zero();
	}
}
