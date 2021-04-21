#include "Particle.h"

Particles::Particle::Particle() : life(0), velocity(0), relativePosition(0), maxLife(5), isAlive(false)
{
}

Particles::Particle::Particle(Vector3 relativePosition, Vector3 velocity, Float maxLife) : Particle()
{
	this->relativePosition = relativePosition;
	this->velocity = velocity;
	this->maxLife = maxLife;
}

void Particles::Particle::update(float deltaTime)
{
	if (NOT isAlive) {
		return;
	}
	relativePosition += velocity * deltaTime;
	life += deltaTime;
	if (life >= maxLife) {
		reset();
	}
}

void Particles::Particle::reset()
{
	isAlive = false;
	life *= 0.0f;
	relativePosition *= 0.0f;
	velocity *= 0.0f;
}

void Particles::Particle::cleanUp()
{
}

void Particles::Particle::spawn(Vector3 vel)
{
	reset();
	velocity = vel;
	isAlive = true;
}

void Particles::Particle::setRelativePosition(Vector3 pos)
{
	relativePosition = pos;
}

void Particles::Particle::setVelocity(Vector3 vel)
{
	velocity = vel;
}

Vector3 Particles::Particle::getRelativePosition() const
{
	return relativePosition;
}

Vector3 Particles::Particle::getVelocity() const
{
	return velocity;
}

Float Particles::Particle::getLife() const
{
	return life;
}

const bool Particles::Particle::getIsAlive() const
{
	return isAlive;
}

void Particles::Particle::addVelocity(Vector3 vel)
{
	velocity += vel;
}
