#include "Buffers.h"
#include <string>
#include <iostream>
#include "../Utils/General.h"
#include "../Utils/ResourceLoader.h"
#include "Mesh.h"
#include "Vertex.h"

char Primative::StaticBuffer::usedBindingPoint = -1;

Primative::VertexBuffer::VertexBuffer(/*const*/ Mesh& mesh, GLenum shape_type, GLenum draw_type, String name = "") : VertexBuffer()
{
	this->name = name;
	this->drawType = draw_type;
	this->shape_type = shape_type;
	num_indices = static_cast<int>(mesh.indices.size());

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	this->bind();

	/*for (int i = 0; i < mesh.verts.size(); i++) {
		auto& v = mesh.verts[i];
		v.ids = glm::vec4(0, 0, 0, 1);
	}*/

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Primative::Vertex), &mesh.verts[0], drawType);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned), &mesh.indices[0], GL_STATIC_DRAW);

	int stride = sizeof(Vertex);
	// layouts in shader
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	// tex
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// norm
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// bone weights
	// part 1
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// part 2
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);
	/*
	// bone weights
	// part 1
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// part 2
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

String Primative::VertexBuffer::getName() const
{
	return name;
}

void Primative::VertexBuffer::setName(String name)
{
	this->name = name;
}

void Primative::VertexBuffer::cleanUp()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

Primative::StaticBuffer::StaticBuffer(const std::vector<std::string>& types, char bindingPoint) : StaticBuffer() {
	unsigned len = 0, prev = 0;;
	for (const std::string& type : types) {
		// floats
		if (type == "f") {
			len = sizeof(float);
		}
		else if (type == "f2") {
			len = sizeof(glm::vec2);
		}
		else if (type == "f3") {
			len = sizeof(glm::vec3);
		}
		else if (type == "f4") {
			len = sizeof(glm::vec4);
		}
		// ints
		else if (type == "i") {
			len = sizeof(int);
		}
		else if (type == "i2") {
			len = sizeof(glm::ivec2);
		}
		else if (type == "i3") {
			len = sizeof(glm::ivec3);
		}
		else if (type == "i4") {
			len = sizeof(glm::ivec4);
		}
		// matrices
		else if (type == "m2") {
			len = sizeof(glm::mat2);
		}
		else if (type == "m3") {
			len = sizeof(glm::mat3);
		}
		else if (type == "m4") {
			len = sizeof(glm::mat4);
		}
		positions.push_back({ prev, len });
		prev += len;
	}
	init(prev, bindingPoint);
}
Primative::StaticBuffer::StaticBuffer(const std::string& types, char bindingPoint) : StaticBuffer(Utils::split(types, ", "), bindingPoint)
{
}
const void Primative::StaticBuffer::init(unsigned dataSize, short bindingPoint)
{
	this->bindingPoint = (bindingPoint < 0) ? Primative::StaticBuffer::usedBindingPoint + 1 : bindingPoint;
	this->bindingPoint %= GL_MAX_UNIFORM_BUFFER_BINDINGS;
	Primative::StaticBuffer::usedBindingPoint = this->bindingPoint;
	glGenBuffers(1, &UBO);
	this->bind();
	glBufferData(GL_UNIFORM_BUFFER, dataSize, NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, this->bindingPoint, UBO, 0, dataSize);
}
void Primative::StaticBuffer::fill(unsigned offset, unsigned size, const void* data) const
{
	this->bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Primative::StaticBuffer::fill(unsigned position, const void* data) const
{
	const std::pair<unsigned, unsigned>& pair = positions[position];
	fill(pair.first, pair.second, data);
}

void Primative::StaticBuffer::cleanUp()
{
	glDeleteBuffers(1, &UBO);
}

const std::tuple<GLenum, int, std::string> Primative::FrameBuffer::getTexType(const std::string& t, char& colTypes)
{
	std::tuple<GLenum, int, std::string> res;
	GLenum& attachmentType = std::get<GLenum>(res) = 0;
	int& type = std::get<int>(res) = -1;
	std::string& t_ = std::get<std::string>(res) = t;

	if (t == "col") {
		attachmentType = GL_COLOR_ATTACHMENT0 + colTypes;
		type = GL_RGB;
		t_ += std::to_string(colTypes++);
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
		t_ = "failed";
	}
	return res;
}

Primative::FrameBuffer::FrameBuffer(std::vector<std::string> textureTypes, glm::ivec2 dimentions) : FrameBuffer()
{
	this->dimentions = dimentions;
	glGenFramebuffers(1, &FBO);
	this->bind();

	if (textureTypes.size() < 0) {
		textureTypes.push_back("col");
	}

	char colTypes = 0;
	// char colTypes_ = 0;
	for (short i = 0; i < textureTypes.size(); i++) {
		std::string t = textureTypes[i];

		const auto r = getTexType(t, colTypes);
		GLenum attachmentType = std::get<GLenum>(r);
		GLenum type = std::get<int>(r);
		if (t == "failed") {
			continue;
		}
		t = std::get<std::string>(r);

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
		std::cout << "FBO failed to complete\n";
	}
	this->unBind();
}

void Primative::FrameBuffer::cleanUp()
{
	if (FBO == 0)
		return;
	this->unBind();
	for (auto& pair : textures) {
		unsigned& id = pair.second; 
		const std::string& t = pair.first;
		char f = 0;
		const auto r = getTexType(t, f);
		GLenum attachmentType = std::get<GLenum>(r);
		// GLenum type = std::get<int>(r);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, 0, 0);
		glDeleteTextures(1, &id);
	}
	textures.clear();

	for (unsigned& rbo : renderBuffers) {
		glDeleteRenderbuffers(1, &rbo);
	}
	renderBuffers.clear();

	glDeleteFramebuffers(1, &FBO);
};

const unsigned& Primative::FrameBuffer::getTextureId(const std::string& name)
{
	unsigned s = textures.size();
	const unsigned& r = textures[name];
	if (s < textures.size()) {
		textures.erase(name);
		return 0;
	}
	return r;
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
	delete soundData;
}

void Primative::SoundBuffer::cleanUp()
{
	alDeleteBuffers(1, &SBO);
}

Primative::MSAABuffer::MSAABuffer(std::vector<std::string> textureTypes, glm::ivec2 dimentions) : FrameBuffer(), middleMan(DBG_NEW FrameBuffer(textureTypes, dimentions))
{
	this->dimentions = dimentions;
	glGenFramebuffers(1, &FBO);
	this->bind();

	if (textureTypes.size() < 0) {
		textureTypes.push_back("col");
	}

	char colTypes = 0;
	// char colTypes_ = 0;
	for (short i = 0; i < textureTypes.size(); i++) {
		std::string t = textureTypes[i];

		const auto r = getTexType(t, colTypes);
		GLenum attachmentType = std::get<GLenum>(r);
		GLenum type = std::get<int>(r);
		if (t == "failed") {
			continue;
		}
		t = std::get<std::string>(r);

		textures.insert({ t, 0 });
		unsigned& tex = textures[t];
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
		// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, type, dimentions.x, dimentions.y, GL_TRUE);

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
	}

	if (!Utils::contains(textureTypes, { "col" })) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	if (!Utils::contains(textureTypes, { "depth" })) {
		unsigned rbo = 0;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH_COMPONENT, dimentions.x, dimentions.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, rbo);
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
		std::cout << "FBO failed to complete\n";
	}
	this->unBind();
}

void Primative::MSAABuffer::bind() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, middleMan->getFBO());
	glBlitFramebuffer(0, 0, dimentions.x, dimentions.y, 0, 0, dimentions.x, dimentions.y, GL_COLOR_BUFFER_BIT, GL_LINEAR); // GL_NEAREST
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	FrameBuffer::bind();
};

void Primative::MSAABuffer::unBind() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	if(middleMan)
		middleMan->unBind();
}

void Primative::MSAABuffer::cleanUp()
{
	middleMan->cleanUp();
	delete middleMan;
	middleMan = nullptr;
	if (FBO == 0)
		return;
	this->unBind();
	for (auto& pair : textures) {
		unsigned& id = pair.second;
		const std::string& t = pair.first;
		char f = 0;
		const auto r = getTexType(t, f);
		GLenum attachmentType = std::get<GLenum>(r);
		// GLenum type = std::get<int>(r);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, 0, 0);
		glDeleteTextures(1, &id);
	}
	textures.clear();

	for (unsigned& rbo : renderBuffers) {
		glDeleteRenderbuffers(1, &rbo);
	}
	renderBuffers.clear();

	glDeleteFramebuffers(1, &FBO);
}

const unsigned& Primative::MSAABuffer::getTextureId(const std::string& name)
{
	/*if (Utils::contains(name, "col")) {
		glViewport(0, 0, dimentions.x, dimentions.y);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, middleMan->getFBO());
		glBlitFramebuffer(0, 0, dimentions.x, dimentions.y, 0, 0, dimentions.x, dimentions.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		unBind();
	}*/
	return middleMan->getTextureId(name);
}
