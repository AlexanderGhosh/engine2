#include "AudioSource.h"
#include "../GameObject/GameObject.h"

void Component::AudioSource::update(float deltaTime)
{
	// update buffers position
	const glm::vec3& pos = parent->getTransform()->Position;
	const Component::ComponetBase* rb = parent->getRigidbody();
	if (rb) {
		const glm::vec3& vel = { 0, 0, 0 };
		alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
	}
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
}

void Component::AudioSource::addBuffer(Primative::SoundBuffer* buffer) const
{
	alSourcei(source, AL_BUFFER, buffer->getSBO());
}

void Component::AudioSource::cleanUp()
{
	alDeleteSources(1, &source);
}
