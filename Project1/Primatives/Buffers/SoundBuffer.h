#pragma once
#include "../../Utils/General.h"
namespace Primative {
	namespace Buffers {
		class SoundBuffer {
		private:
			unsigned SBO; // sound buffer object
			float bitDepth, sampleRate, dataSize;
		public:
			SoundBuffer();
			SoundBuffer(const char* soundData, Int len, Unsigned channel, Int sampleRate, Unsigned bitDepth);
			~SoundBuffer() = default;
			virtual void update(unsigned source);
			void cleanUp();
			virtual void setSource(Unsigned s);
			Unsigned getSBO() const;
		};
	}
}
