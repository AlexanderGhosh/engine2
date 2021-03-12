#include "SoundStreamBuffer.h"

Primative::Buffers::SoundStreamBuffer::SoundStreamBuffer() : SBOs(), name(), bitDepth(), sampleRate(), format(AL_FORMAT_MONO8), soundData(nullptr), dataSize(0), cursor(0)
{
}

Primative::Buffers::SoundStreamBuffer::SoundStreamBuffer(const char* soundData, Int len, Unsigned channel, Int sampleRate, Unsigned bitDepth) : SoundStreamBuffer()
{
	this->sampleRate = sampleRate;
	dataSize = len;
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

	alGenBuffers(MAX_SOUND_BUFFERS, &SBOs[0]);
	for (char i = 0; i < MAX_SOUND_BUFFERS; ++i)
	{
		alBufferData(SBOs[i], format, &soundData[i * SOUND_BUFFER_SIZE], SOUND_BUFFER_SIZE, sampleRate); // Asumes the data is divisable by MAX_SOUND_BUFFERS
	}
	this->soundData = new char[len];
	std::memcpy(this->soundData, soundData, len); // populates this->soundData with the soundData
	delete soundData;
}

void Primative::Buffers::SoundStreamBuffer::update(unsigned source)
{
	ALint buffersProcessed = 0;
	alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);

	if (buffersProcessed <= 0)
		return;

	while (buffersProcessed--)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(source, 1, &buffer);

		ALsizei dataSize = SOUND_BUFFER_SIZE;

		char* data = new char[dataSize];
		std::memset(data, 0, dataSize);

		std::size_t dataSizeToCopy = SOUND_BUFFER_SIZE;
		if (cursor + SOUND_BUFFER_SIZE > dataSize)
			dataSizeToCopy = dataSize - cursor;

		std::memcpy(&data[0], &soundData[cursor], dataSizeToCopy);
		cursor += dataSizeToCopy;

		if (dataSizeToCopy < SOUND_BUFFER_SIZE)
		{
			cursor = 0;
			std::memcpy(&data[dataSizeToCopy], &soundData[cursor], SOUND_BUFFER_SIZE - dataSizeToCopy);
			cursor = SOUND_BUFFER_SIZE - dataSizeToCopy;
		}

		alBufferData(buffer, format, data, SOUND_BUFFER_SIZE, sampleRate);
		alSourceQueueBuffers(source, 1, &buffer);

		delete[] data;
	}
}

void Primative::Buffers::SoundStreamBuffer::setSource(Unsigned s)
{
	alSourceQueueBuffers(s, MAX_SOUND_BUFFERS, &SBOs[0]);
}

void Primative::Buffers::SoundStreamBuffer::cleanUp()
{
	alDeleteBuffers(MAX_SOUND_BUFFERS, &SBOs[0]);
	delete[] soundData;
}
