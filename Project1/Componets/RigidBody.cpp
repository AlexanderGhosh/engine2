#include "Rigidbody.h"
#include "../Physics/Engine.h"
#include "../GameObject/GameObject.h"

Component::Rigidbody::Rigidbody() : velocity(), collider(), pos(nullptr), isKinimatic(false)
{
}

Component::Rigidbody::Rigidbody(bool isKinimatic) : Rigidbody()
{
	this->isKinimatic = isKinimatic;
	Physics::Engine::addRigidbody(this);
}

void Component::Rigidbody::cleanUp()
{
}

void Component::Rigidbody::setCollider(Physics::SphereCollider& collider)
{
	this->collider = collider;
}

void Component::Rigidbody::setParent(GameObject* parent)
{
	Component::ComponetBase::setParent(parent);
	this->pos = &parent->getLocalTransform()->Position;
}

void Component::Rigidbody::update(float deltaTime)
{
	intergrateForPosition(deltaTime);
}

void Component::Rigidbody::backPeddle(float d)
{
	if (glm::all(glm::equal(velocity, { 0, 0, 0 }))) {
		return;
	}
	auto delta = glm::normalize(velocity) * d;
	*pos -= delta;
}

const float Component::Rigidbody::getInvMass() const
{
	return collider.getInvMass();
}

const glm::mat3 Component::Rigidbody::getGlobalInvInertiaTensor() const
{
	return collider.getGlobalInvInertiaTensor();
}

void Component::Rigidbody::applyForce(glm::vec3 force)
{
	if (isKinimatic) {
		return;
	}
	force *= getInvMass();
	velocity += force;
}

void Component::Rigidbody::intergrateForPosition(const float dt)
{
	*pos += velocity * dt;
}
