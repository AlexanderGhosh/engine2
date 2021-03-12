#pragma once
#include <al.h>
#include "../../Utils/General.h"
#include "SoundBuffer.h"
namespace Primative {
	namespace Buffers {
		class SoundStreamBuffer : public SoundBuffer {
#define MAX_SOUND_BUFFERS 4
#define SOUND_BUFFER_SIZE 65536 // 32kb of data in each buffer
		private:
			std::array<unsigned, MAX_SOUND_BUFFERS> SBOs;
			// unsigned SBO; // sound buffer object
			std::string name;
			float bitDepth, sampleRate;
			ALenum format;
			char* soundData;
			unsigned cursor, dataSize;
		public:
			SoundStreamBuffer();
			SoundStreamBuffer(const char* soundData, Int len, Unsigned channel, Int sampleRate, Unsigned bitDepth);
			~SoundStreamBuffer() = default;
			void update(unsigned source);
			void setSource(Unsigned s);
			void cleanUp();
		};
	}
}
