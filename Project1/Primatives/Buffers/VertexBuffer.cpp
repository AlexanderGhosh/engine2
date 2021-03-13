#include "VertexBuffer.h"
#include "../Mesh.h"
#include "../Vertex.h"

#pragma region Constructors Cleanup

Primative::Buffers::VertexBuffer::VertexBuffer() : VBO(0), VAO(0), EBO(0), num_indices(0), shape_type(GL_TRIANGLES), drawType(GL_STATIC_DRAW), name("")
{
}

Primative::Buffers::VertexBuffer::VertexBuffer(Mesh& mesh, GLenum shape_type, GLenum draw_type, std::string name) : VertexBuffer()
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
	glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * sizeof(Vertex), &mesh.verts[0], drawType);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned), &mesh.indices[0], GL_STATIC_DRAW);

	int stride = sizeof(Vertex);
	// layouts in shader
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	// tex
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, tex));
	glEnableVertexAttribArray(1);
	// norm
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, norm));
	glEnableVertexAttribArray(2);

	// bone ids
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, ids));
	glEnableVertexAttribArray(3);
	// bone weights
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, weights));
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Primative::Buffers::VertexBuffer::cleanUp()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
#pragma endregion

#pragma region Rendering

void Primative::Buffers::VertexBuffer::bind() const
{
	glBindVertexArray(VAO);
}

void Primative::Buffers::VertexBuffer::unBind() const
{
	glBindVertexArray(0);
}

void Primative::Buffers::VertexBuffer::draw() const
{
	glDrawElements(shape_type, num_indices, GL_UNSIGNED_INT, 0);
}

void Primative::Buffers::VertexBuffer::render() const
{
	bind();
	draw();
	unBind();
}
#pragma endregion

#pragma region Getters
Unsigned Primative::Buffers::VertexBuffer::getVBO() const
{
	return VBO;
}

Unsigned Primative::Buffers::VertexBuffer::getVAO() const
{
	return VAO;
}

Unsigned Primative::Buffers::VertexBuffer::getEBO() const
{
	return EBO;
}
Unsigned Primative::Buffers::VertexBuffer::getIndicesCount() const
{
	return num_indices;
}
const GLenum& Primative::Buffers::VertexBuffer::getDrawType() const
{
	return shape_type;
}
#pragma endregion

#pragma region Setters

void Primative::Buffers::VertexBuffer::setDrawType(GLenum type)
{
	drawType = type;
}

String Primative::Buffers::VertexBuffer::getName() const
{
	return name;
}

void Primative::Buffers::VertexBuffer::setName(String name)
{
	this->name = name;
}
#pragma endregion