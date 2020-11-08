#include "AudioSource.h"
#include "../GameObject/GameObject.h"

void Componet::AudioSource::update()
{
	// update buffers position
	const glm::vec3& pos = parent->getTransform()->Position;
	const Componet::Base* rb = parent->getRigidbody();
	if (rb) {
		const glm::vec3& vel = { 0, 0, 0 };
		alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
	}
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
}

void Componet::AudioSource::addBuffer(Primative::SoundBuffer* buffer) const
{
	alSourcei(source, AL_BUFFER, buffer->getSBO());
}
