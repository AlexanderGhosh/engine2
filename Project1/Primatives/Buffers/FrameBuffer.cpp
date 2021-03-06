#include "FrameBuffer.h"


Primative::Buffers::FrameBuffer::FrameBuffer() : fbo(), textures(), backgroundColour(), dimentions(), clearMask(GL_DEPTH_BUFFER_BIT)
{
}

Primative::Buffers::FrameBuffer::FrameBuffer(const std::vector<std::string>& textures, const glm::ivec2& dimentions, const glm::vec3& bgColour) : FrameBuffer()
{
	this->dimentions = dimentions;
	this->backgroundColour = bgColour;
	for (String tex : textures) {
		this->textures[tex] = 0;
	}
	if (Utils::contains(textures, std::string("col0"))) {
		clearMask = clearMask | GL_COLOR_BUFFER_BIT;
	}
	if (Utils::contains(textures, std::string("stencil"))) {
		clearMask = clearMask | GL_STENCIL_BUFFER_BIT;
	}
	if (Utils::contains(textures, std::string("depth_stencil"))) {
		clearMask = clearMask | GL_STENCIL_BUFFER_BIT;
	}

	glGenFramebuffers(1, &fbo);
	bind();
	clearBits();
	initalize();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to create FBO" << std::endl;
	}
	unBind();


}

void Primative::Buffers::FrameBuffer::initalize()
{
	unsigned colAttach = 0;
	bool has_colour = false;
	bool has_depth = false;
	for (auto itt = textures.begin(); itt != textures.end(); itt++) {
		String type = (*itt).first;
		unsigned& tex = (*itt).second;

		std::array<int, 4> data = getBufferData(type, colAttach);
		if (data[0] < 0)
			continue;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, data[0], dimentions.x, dimentions.y, 0, data[1], data[3], NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, data[2], GL_TEXTURE_2D, tex, 0);

		if (Utils::contains(type, "col"))
			has_colour = true;
		if (type == "depth")
			has_depth = true;
	}
	if (NOT has_colour) {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	if (NOT has_depth) {
		glGenRenderbuffers(1, &depthStencilRBO);

		glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimentions.x, dimentions.y);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRBO);
	}
}

std::array<int, 4> Primative::Buffers::FrameBuffer::getBufferData(String type, unsigned& colAttach) const
{
	std::array<int, 4> res{};
	if (Utils::contains(type, "col")) {
		res[0] = GL_RGB;
		res[1] = GL_RGB;
		res[2] = GL_COLOR_ATTACHMENT0 + colAttach++;
		res[3] = GL_UNSIGNED_BYTE;
	}
	else if (type == "depth") {
		res[0] = GL_DEPTH_COMPONENT;
		res[1] = GL_DEPTH_COMPONENT;
		res[2] = GL_DEPTH_ATTACHMENT;
		res[3] = GL_FLOAT;
	}
	/*else if (type == "stencil") {
		res[0] = GL_INT;
		res[1] = GL_INT;
		res[2] = GL_STENCIL_ATTACHMENT;
	}*/
	else if (type == "depth_stencil") {
		res[0] = GL_DEPTH24_STENCIL8;
		res[1] = GL_DEPTH24_STENCIL8;
		res[2] = GL_DEPTH_STENCIL_ATTACHMENT;
		res[3] = GL_UNSIGNED_INT_24_8;
	}
	else {
		res[0] = -1;
	}
	return res;
}

void Primative::Buffers::FrameBuffer::cleanUp()
{
	for (auto itt = textures.begin(); itt != textures.end();) {
		glDeleteTextures(1, &(*itt).second);
		itt = textures.erase(itt);
	}
	glDeleteRenderbuffers(1, &depthStencilRBO);
	glDeleteFramebuffers(1, &fbo);
}

void Primative::Buffers::FrameBuffer::bind() const
{
	glViewport(0, 0, dimentions.x, dimentions.y);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Primative::Buffers::FrameBuffer::unBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Primative::Buffers::FrameBuffer::clearBits() const
{
	glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, 1);
	glClear(clearMask);
}

Unsigned Primative::Buffers::FrameBuffer::getTexture(String name)
{
	const int s = textures.size();
	Unsigned res = textures[name];
	if (s != textures.size()) {
		textures.erase(name);
		return 0;
	}
	return res;
}
