#include "SoundBuffer.h"
#include <al.h>

#pragma region Constructors CleanUp
Primative::Buffers::SoundBuffer::SoundBuffer() : SBO(0), name(), bitDepth(0), sampleRate(0), dataSize()
{
}

Primative::Buffers::SoundBuffer::SoundBuffer(const char* soundData, Int len, Unsigned channel, Int sampleRate, Unsigned bitDepth) : SoundBuffer()
{
	alGenBuffers(1, &SBO);
	ALenum format = AL_FORMAT_MONO8; // 0x1100
	char channels[] = {
		1, 2
	};
	char bitDepths[] = {
		8, 16
	};

	bool found = 0;
	for (char i = 0; i < 2; i++) {
		for (char j = 0; j < 2; j++) {
			if (channels[i] == channel && bitDepths[j] == bitDepth) {
				found = 1;
				break;
			}
			format++;
		}
	}

	if (!found)
	{
		std::cerr
			<< "ERROR: unrecognised wave format: "
			<< channels << " channels, "
			<< bitDepth << " bps" << std::endl;
		return;
	}

	alBufferData(SBO, format, soundData, len, sampleRate);
	delete soundData;
}

void Primative::Buffers::SoundBuffer::cleanUp()
{
	alDeleteBuffers(1, &SBO);
}
#pragma endregion

#pragma region Getters
Unsigned Primative::Buffers::SoundBuffer::getSBO() const
{
	return SBO;
}
#pragma endregion

