#pragma once
#include <GL/glew.h>
#include "Mesh.h"
namespace Primative {
	class Buffers
	{
	private:
		unsigned VBO, VAO, EBO;
		unsigned num_indices;
		GLenum shape_type, drawType;
	public:
		inline const unsigned getVBO() const { return VBO; };
		inline const unsigned getVAO() const { return VAO; };
		inline const unsigned getEBO() const { return EBO; };

		inline Buffers() : VBO(0), VAO(0), EBO(0), num_indices(0), shape_type(GL_TRIANGLES), drawType(GL_STATIC_DRAW) { };
		Buffers(const Mesh& mesh);
		inline void bind() const { glBindVertexArray(VAO); };
		inline void draw() const { glDrawElements(shape_type, num_indices, GL_UNSIGNED_INT, 0); };
	};
}

