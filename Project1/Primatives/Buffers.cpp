#include "Buffers.h"
#include <string>
#include <iostream>

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
