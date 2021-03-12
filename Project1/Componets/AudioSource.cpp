#include "AudioSource.h"
#include "../Primatives/Buffers/SoundBuffer.h"
#include "../GameObject/GameObject.h"
#include "../Componets/RigidBody.h"
#include "../Primatives/Buffers/SoundStreamBuffer.h"

Component::AudioSource::AudioSource() : ComponetBase(), source(0), isLooping(false), buffer(nullptr)
{ 

}

Component::AudioSource::AudioSource(unsigned source) : AudioSource()
{
	this->source = source;
}

void Component::AudioSource::update(float deltaTime)
{
	if(isPlaying())
		buffer->update(source);
	// update buffers position
	Vector3 pos = parent->getTransform()->Position;
	const Component::RigidBody* rb = parent->getRigidbody();
	if (rb) {
		Vector3 vel = rb->getVelocity();
		alSource3f(source, AL_VELOCITY, vel.x, vel.y, vel.z);
	}
	alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
	if (isLooping AND NOT isPlaying()) {
		play();
	}
}

void Component::AudioSource::play() const
{
	alSourcePlay(source);
}

void Component::AudioSource::pause()
{
	isLooping = false;
	alSourcePause(source);
}

void Component::AudioSource::togglePlay()
{
	if (isPlaying()) {
		pause();
	}
	else {
		play();
	}
}

void Component::AudioSource::toggleLoop()
{
	isLooping = NOT isLooping;
}

bool Component::AudioSource::isPlaying() const
{
	int res = 0;
	alGetSourcei(source, AL_SOURCE_STATE, &res);
	return res == AL_PLAYING;
}

void Component::AudioSource::addBuffer(Primative::Buffers::SoundBuffer* buffer)
{
	if (NOT buffer)
		return;
	if(NOT dynamic_cast<Primative::Buffers::SoundStreamBuffer*>(buffer))
		alSourcei(source, AL_BUFFER, buffer->getSBO());
	this->buffer = buffer;
	buffer->setSource(source);
}

void Component::AudioSource::cleanUp()
{
	alDeleteSources(1, &source);
}
