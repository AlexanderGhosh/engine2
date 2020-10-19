#include "Buffers.h"
#include <string>
#include <iostream>

short Primative::StaticBuffer::usedBindingPoint = -1;

Primative::Buffers::Buffers(const Mesh& mesh) : Buffers()
{
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
