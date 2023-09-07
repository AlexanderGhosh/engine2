#include "Rigidbody.h"
#include "../Physics/Engine.h"
#include "../GameObject/GameObject.h"
#include "../Physics/Collision/Colliders/SATBaseCollider.h"

Component::Rigidbody::Rigidbody() : velocity(), collider(), pos(nullptr), isKinimatic(false), angularVelocity(0), hasGravity_(true)
{
}

Component::Rigidbody::Rigidbody(bool isKinimatic, bool hasGravity) : Rigidbody()
{
	this->isKinimatic = isKinimatic;
	this->hasGravity_ = hasGravity;
	Physics::Engine::addRigidbody(this);
}

void Component::Rigidbody::cleanUp()
{
}

void Component::Rigidbody::setCollider(Physics::Collider& collider)
{
	this->collider = &collider;
}

void Component::Rigidbody::setParent(GameObject* parent)
{
	Component::ComponetBase::setParent(parent);
	pos = &(parent->getLocalTransform()->Position);
	rot = &(parent->getLocalTransform()->Rotation);
}

void Component::Rigidbody::update(float deltaTime)
{
	if (isKinimatic) return;
	if (!glm::all(glm::equal(netTorque, glm::vec3(0)))) {
		int i = 0;
	}
	intergrateForVelocity(deltaTime);
	intergrateForPosition(deltaTime);
	applyGravity(Utils::yAxis(-.1f));
}

void Component::Rigidbody::backPeddle(float d, glm::vec3 n)
{
	if (glm::all(glm::equal(velocity, { 0, 0, 0 }))) {
		return;
	}
	auto delta = glm::normalize(velocity) * d;
	*pos -= d * n;
}

const glm::vec3 Component::Rigidbody::getVelocity() const
{
	if (isKinimatic) {
		return glm::vec3(0);
	}
	return velocity;
}

const glm::vec3 Component::Rigidbody::getAngularVelocity() const
{
	if (isKinimatic) {
		return glm::vec3(0);
	}
	return angularVelocity;
}

const float Component::Rigidbody::getInvMass() const
{
	if (isKinimatic) return 0.f;
	return collider->getInvMass();
}

const glm::mat3 Component::Rigidbody::getGlobalInvInertiaTensor() const
{
	return collider->getGlobalInvInertiaTensor();
}

void Component::Rigidbody::applyForce(Vector3 force, Vector3 pos)
{
	if (isKinimatic) {
		return;
	}
	netForce += force;
	netTorque += glm::cross(pos, force);
	// velocity += j * getInvMass() * n;
	// angularVelocity += getGlobalInvInertiaTensor() * ;
}

void Component::Rigidbody::applyGravity(Vector3 force)
{
	applyForce(force / getInvMass(), glm::vec3(0));
}

void Component::Rigidbody::intergrateForPosition(const float dt)
{
	if (isKinimatic) return;
	*pos += velocity * dt;
	*rot *= glm::quat(angularVelocity * dt);
	*rot = glm::normalize(*rot);
}

void Component::Rigidbody::intergrateForVelocity(const float dt)
{
	velocity += netForce * getInvMass() * dt;
	angularVelocity += netTorque * getGlobalInvInertiaTensor() * dt;

	netForce *= 0;
	netTorque *= 0;
}
