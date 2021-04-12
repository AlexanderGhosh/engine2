#include "CharacterController.h"
#include "../GameObject/GameObject.h"
#include "../Physics/Engine.h"

Component::CharacterController::CharacterController() : ComponetBase(), velocity(0), position(nullptr), hasGravity(true), isGrounded(false)
{
}

void Component::CharacterController::cleanUp()
{
}

void Component::CharacterController::intergrateVelocity(Float deltaTime)
{
	if (hasGravity) {
		velocity += Physics::Engine::getGravity() * deltaTime;
	}
	if (isGrounded) {
		velocity -= Physics::Engine::getGravity() * deltaTime;
	}
	(*position) += velocity * deltaTime;
}

Vector3 Component::CharacterController::getVelocity() const
{
	return velocity;
}

Vector3 Component::CharacterController::getPosition() const
{
	return *position;
}

void Component::CharacterController::setParent(GameObject* parent)
{
	ComponetBase::setParent(parent);
	position = &parent->getTransform()->Position;
}

void Component::CharacterController::setVelocity(Vector3 vel)
{
	velocity = vel;
}

void Component::CharacterController::setPosition(Vector3 pos)
{
	(*position) = pos;
}

void Component::CharacterController::setIsGrounded(const bool grounded)
{
	isGrounded = grounded;
}

void Component::CharacterController::addVelocity(Vector3 vel)
{
	this->velocity += vel;
}

void Component::CharacterController::addPosition(Vector3 pos)
{
	(*position) += pos;
}

void Component::CharacterController::update(float deltaTime)
{
	intergrateVelocity(deltaTime);
}
