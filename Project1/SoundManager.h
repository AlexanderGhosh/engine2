#pragma once
#include <glm.hpp>
#include <alc.h>
#include "Utils/General.h"

namespace Primative {
	namespace Buffers {
		class SoundBuffer;
		class SoundStreamBuffer;
	}
}

class SoundManager
{
private:
	SoundManager();
	static ALCdevice* device;
	static ALCcontext* contex;
	static std::vector<Primative::Buffers::SoundBuffer> buffers;
	static std::vector<Primative::Buffers::SoundStreamBuffer> streams;
public:
	static bool init();
	static void setRecieverInfo(const glm::vec3& pos = Utils::zero(), const glm::vec3& lookDir = Utils::zAxis(-1), const glm::vec3& velocity = Utils::zero(), const glm::vec3& up = Utils::yAxis());
	static Primative::Buffers::SoundBuffer* createBuffer(const std::string& filePath);
	static Primative::Buffers::SoundStreamBuffer* createBuffer(const std::string& filePath, bool isStream);
	static unsigned createSoundSource();
	static void cleanUp();
};

