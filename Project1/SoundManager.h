#pragma once
#include <glm.hpp>
#include <alc.h>
#include "Primatives/Buffers.h"
#include "Utils/General.h"
class SoundManager
{
private:
	SoundManager();
	static ALCdevice* device;
	static ALCcontext* contex;
	static std::vector<Primative::SoundBuffer> buffers;
public:
	static bool init();
	static void setRecieverInfo(const glm::vec3& pos = Utils::zero(), const glm::vec3& lookDir = Utils::zAxis(-1), const glm::vec3& velocity = Utils::zero(), const glm::vec3& up = Utils::yAxis());
	static Primative::SoundBuffer* createBuffer(const std::string& filePath);
	static unsigned createSoundSource();
	static void cleanUp();
};

