#include "Buffers.h"
#include <string>
#include <iostream>
#include "../Utils/General.h"

short Primative::StaticBuffer::usedBindingPoint = -1;

Primative::Buffers::Buffers(const Mesh& mesh, GLenum shape_type) : Buffers()
{
	this->shape_type = shape_type;
	num_indices = static_cast<int>(mesh.indices.size());

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	this->bind();

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Primative::Vertex), &mesh.verts[0], drawType);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned), &mesh.indices[0], GL_STATIC_DRAW);

	// layouts in shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

const unsigned short Primative::StaticBuffer::init(unsigned dataSize, short bindingPoint)
{
	this->bindingPoint = (bindingPoint < 0) ? Primative::StaticBuffer::usedBindingPoint + 1 : bindingPoint;
	this->bindingPoint %= GL_MAX_UNIFORM_BUFFER_BINDINGS;
	Primative::StaticBuffer::usedBindingPoint = this->bindingPoint;
	glGenBuffers(1, &UBO);
	this->bind();
	glBufferData(GL_UNIFORM_BUFFER, dataSize, NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, this->bindingPoint, UBO, 0, dataSize);
	return 0;
}
void Primative::StaticBuffer::fill(unsigned offset, unsigned size, const void* data) const
{
	this->bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Primative::FrameBuffer::FrameBuffer(std::vector<std::string> textureTypes, glm::ivec2 dimentions) : FrameBuffer()
{
	this->dimentions = dimentions;
	glGenFramebuffers(1, &FBO);
	this->bind();

	if (textureTypes.size() < 0) {
		textureTypes.push_back("col");
	}

	unsigned colTypes = 0;
	for (short i = 0; i < textureTypes.size(); i++) {
		GLenum attachmentType, type;
		std::string t = textureTypes[i];
		if (t == "col") {
			attachmentType = GL_COLOR_ATTACHMENT0 + colTypes;
			type = GL_RGB;
			t += std::to_string(colTypes++);
		}
		else if (t == "depth") {
			attachmentType = GL_DEPTH_ATTACHMENT;
			type = GL_DEPTH_COMPONENT;
		}
		else if (t == "stencil") {
			attachmentType = GL_STENCIL_ATTACHMENT;
			type = GL_FLOAT;
		}
		else if (t == "dep_sten") {
			attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
			type = GL_DEPTH24_STENCIL8;
		}
		else {
			continue;
		}

		textures.insert({ t, 0 });
		unsigned& tex = textures[t];
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, type, dimentions.x, dimentions.y, 0, type, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, tex, 0);
	}
	
	if (!Utils::contains(textureTypes, { "col" })) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	if (!Utils::contains(textureTypes, { "depth" })) {
		unsigned rbo = 0;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, dimentions.x, dimentions.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		renderBuffers.push_back(rbo);
	}
	/*if (!Utils::contains(textureTypes, { "stencil" })) {
		unsigned rbo = 0;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_UNSIGNED_BYTE, dimentions.x, dimentions.y);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		renderBuffers.push_back(rbo);
	}*/
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "error\n";
	}
	this->unBind();
}

const unsigned& Primative::FrameBuffer::getTextureId(const std::string& name)
{
	unsigned s = textures.size();
	const unsigned& r = textures[name];
	bool b = true;
	if (s < textures.size()) {
		textures.erase(name);
		b = false;
	}
	if (b) 
		return r;
	return 0;
}

Primative::SoundBuffer::SoundBuffer(const char* soundData, const int& len, const unsigned& channel, const int& sampleRate, const unsigned& bitDepth) : SoundBuffer()
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

	if(!found)
	{
		std::cerr
			<< "ERROR: unrecognised wave format: "
			<< channels << " channels, "
			<< bitDepth << " bps" << std::endl;
		return;
	}

	alBufferData(SBO, format, soundData, len, sampleRate);

}

void Primative::SoundBuffer::cleanUp()
{
	alDeleteBuffers(1, &SBO);
}
