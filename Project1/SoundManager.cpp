#include "SoundManager.h"
#include <iostream>
#include <filesystem>
#include <bit>
#include <alc.h>
#include <al.h>
#include <Audio/AudioReader.h>
#include "Primatives/Buffers/SoundStreamBuffer.h"
#define NUM_BUFFERS 1

ALCdevice* SoundManager::device = nullptr;
ALCcontext* SoundManager::contex = nullptr;
std::vector<Primative::Buffers::SoundBuffer> SoundManager::buffers = { };
std::vector<Primative::Buffers::SoundStreamBuffer> SoundManager::streams = { };

Primative::Buffers::SoundBuffer* SoundManager::createBuffer(const std::string& filePath)
{
	unsigned channels, bitDepth;
	int sampleRate, len;
	const char* data = FileReaders::load_wav(filePath, channels, sampleRate, bitDepth, len);
	buffers.push_back({ data, len, channels, sampleRate, bitDepth });
	return &buffers.back();
}

Primative::Buffers::SoundStreamBuffer* SoundManager::createBuffer(const std::string& filePath, bool isStream)
{
	unsigned channels, bitDepth;
	int sampleRate, len;
	const char* data = FileReaders::load_wav(filePath, channels, sampleRate, bitDepth, len);
	streams.push_back({ data, len, channels, sampleRate, bitDepth });
	return &streams.back();
}

bool SoundManager::init()
{
	ALCdevice* device = alcOpenDevice(NULL);
	if (!device) {
		std::cout << "Faild to get device" << std::endl;
		return false;
	}
	ALCcontext* contex = alcCreateContext(device, NULL);
	if (!contex) {
		std::cout << "Failed to create contex" << std::endl;
		return false;
	}
	alcMakeContextCurrent(contex);
	setRecieverInfo();
	return true;
}

void SoundManager::setRecieverInfo(const glm::vec3& pos, const glm::vec3& lookDir, const glm::vec3& velocity, const glm::vec3& up)
{
	alListenerf(AL_GAIN, 1);
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	float ori[6] = {
		lookDir.x,
		lookDir.y,
		lookDir.z,
		up.x,
		up.y, 
		up.z
	};
	alListenerfv(AL_ORIENTATION, ori);
}

unsigned SoundManager::createSoundSource()
{
	// CREATES A SOURCE //
	// const Primative::SoundBuffer* buffer = createBuffer(path);
	unsigned source = 0;
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, 0);
	// alSourcei(source, AL_BUFFER, buffer->getSBO());


	// alDeleteSources(1, &source);
	return source;
}

void SoundManager::cleanUp()
{
	while (!buffers.empty()) {
		buffers.back().cleanUp();
		buffers.pop_back();
	}
	for (auto itt = streams.begin(); itt != streams.end();) {
		(*itt).cleanUp();
		itt = streams.erase(itt);
	}

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(contex);
	alcCloseDevice(device);

	delete contex;
	contex = nullptr;
	delete device;
	device = nullptr;
}
