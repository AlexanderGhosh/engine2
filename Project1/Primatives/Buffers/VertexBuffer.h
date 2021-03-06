#pragma once
#include <GL/glew.h>
#include "../../Utils/General.h"
namespace Primative {
	class Mesh;
	namespace Buffers {
		class VertexBuffer
		{
		private:
			unsigned VBO, VAO, EBO;
			unsigned num_indices;
			GLenum shape_type, drawType;
			std::string name;
		public:
			VertexBuffer();
			VertexBuffer(Mesh& mesh, GLenum shape_type = GL_TRIANGLES, GLenum draw_type = GL_STATIC_DRAW, std::string name = "");
			~VertexBuffer() = default;
			void cleanUp();

			// drawing
			void bind() const;
			void unBind() const;
			void draw() const;
			void render() const;

			// getters
			Unsigned getVBO() const;
			Unsigned getVAO() const;
			Unsigned getEBO() const;

			// setters
			void setDrawType(GLenum type);
			String getName() const;
			void setName(String name);
		};
	}
}