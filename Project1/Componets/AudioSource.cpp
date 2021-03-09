#include "AudioSource.h"
#include "../Primatives/Buffers/SoundBuffer.h"
#include "../GameObject/GameObject.h"
#include "../Componets/RigidBody.h"

void Component::AudioSource::update(float deltaTime)
{
	// update buffers position
	Vector3 pos = parent->getTransform()->Position;
	const Component::RigidBody* rb = parent->getRigidbody();
	if (rb) {
		Vector3 vel = rb->getVelocity();
		alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
	}
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
}

void Component::AudioSource::addBuffer(Primative::Buffers::SoundBuffer* buffer) const
{
	alSourcei(source, AL_BUFFER, buffer->getSBO());
}

void Component::AudioSource::cleanUp()
{
	alDeleteSources(1, &source);
}
