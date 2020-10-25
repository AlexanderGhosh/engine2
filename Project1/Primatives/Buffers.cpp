#include "Buffers.h"
#include <string>
#include <iostream>

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
	glGenFramebuffers(1, &FBO);
	this->bind();
	std::unordered_map<std::string, GLenum> conversion = {
		{ "col", GL_COLOR_ATTACHMENT0 }
	};

	std::vector<std::string> req = { "depth", "dep_sten" };

	if (textureTypes.size() < 0) {
		textureTypes.push_back("col");
	}

	unsigned colTypes = 0;
	for (short i = 0; i < textureTypes.size(); i++) {
		GLenum texType, type;
		const std::string& t = textureTypes[i];
		if (t == "col") {
			texType = GL_COLOR_ATTACHMENT0 + colTypes++;
			type = GL_RGB;
		}
		else if (t == "depth") {
			texType = GL_DEPTH_ATTACHMENT;
			type = GL_DEPTH_COMPONENT32;
		}
		else if (t == "stencil") {
			texType = GL_STENCIL_ATTACHMENT;
			type = GL_FLOAT;
		}
		else if (t == "dep_sten") {
			texType = GL_DEPTH_STENCIL_ATTACHMENT;
			type = GL_DEPTH24_STENCIL8;
		}
		else {
			continue;
		}

		unsigned tex = 0;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, type, dimentions.x, dimentions.y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
		textures.insert({ std::to_string(i), tex });

		//glBindTexture(GL_TEXTURE_2D, 0);
	}
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ghnoirs\n";
	}
	this->unBind();
}
