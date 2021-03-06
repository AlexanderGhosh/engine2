#include "ArrayBuffer.h"

#pragma region Constructors CleanUp
Primative::Buffers::TextureBuffer::TextureBuffer() : VBO(0), TAO(0), typeContained(GL_R32F), dataSize()
{

}

Primative::Buffers::TextureBuffer::TextureBuffer(String typeContained, Unsigned dataSize) : TextureBuffer()
{
	this->dataSize = dataSize;
	if (typeContained == "f") {
	}
	else if (typeContained == "i") {
		this->typeContained = GL_R32I;
	}
	else if (typeContained == "f2") {
		this->typeContained = GL_RG32F;
	}
	else if (typeContained == "i2") {
		this->typeContained = GL_RG32I;
	}
	else if (typeContained == "f3") {
		this->typeContained = GL_RGB32F;
	}
	else if (typeContained == "i3") {
		this->typeContained = GL_RGB32I;
	}
	else if (typeContained == "f4") {
		this->typeContained = GL_RGBA32F;
	}
	else if (typeContained == "i4") {
		this->typeContained = GL_RGBA32I;
	}
	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_TEXTURE_BUFFER, VBO);
	glBufferData(GL_TEXTURE_BUFFER, dataSize, NULL, GL_DYNAMIC_DRAW);
	// TAO
	glGenTextures(1, &TAO);
	glBindTexture(GL_TEXTURE_BUFFER, TAO);
	glTexBuffer(GL_TEXTURE_BUFFER, this->typeContained, VBO);
	// un bind
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}

void Primative::Buffers::TextureBuffer::cleanUp()
{
	glDeleteBuffers(1, &VBO);
}
#pragma endregion

#pragma region Fill Bind
void Primative::Buffers::TextureBuffer::fill(const void* data, Unsigned dataSize, int offset)
{
	bind();
	glBufferSubData(GL_TEXTURE_BUFFER, offset, dataSize, data);
	unBind();
}

void Primative::Buffers::TextureBuffer::bind() const
{
	glBindBuffer(GL_TEXTURE_BUFFER, VBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, TAO);
}

void Primative::Buffers::TextureBuffer::unBind() const
{
	glBindTexture(GL_TEXTURE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
#pragma endregion