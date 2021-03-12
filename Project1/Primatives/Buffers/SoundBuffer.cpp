#include "SoundBuffer.h"
#include <al.h>

#pragma region Constructors CleanUp
Primative::Buffers::SoundBuffer::SoundBuffer() : SBO(0), bitDepth(0), sampleRate(0), dataSize(), canAttenuate(false)
{
}

Primative::Buffers::SoundBuffer::SoundBuffer(const char* soundData, Int len, Unsigned channel, Int sampleRate, Unsigned bitDepth) : SoundBuffer()
{
	alGenBuffers(1, &SBO);
	ALenum format = AL_FORMAT_MONO8; // 0x1100 // 4355
	canAttenuate = channel == 1;
	const char* channels = new char[2] {
		1, 2
	};
	const char* bitDepths = new char[2]{
		8, 16
	};

	bool found = 0;
	for (char i = 0; i < 2 && NOT found; i++) {
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
	delete[] soundData;
	delete[] channels;
	delete[] bitDepths;
}

void Primative::Buffers::SoundBuffer::update(unsigned source)
{
}

void Primative::Buffers::SoundBuffer::cleanUp()
{
	alDeleteBuffers(1, &SBO);
}
void Primative::Buffers::SoundBuffer::setSource(Unsigned s)
{
}
#pragma endregion

#pragma region Getters
Unsigned Primative::Buffers::SoundBuffer::getSBO() const
{
	return SBO;
}
bool Primative::Buffers::SoundBuffer::getAttenuate() const
{
	return canAttenuate;
}
#pragma endregion

