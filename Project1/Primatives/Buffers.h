#pragma once
#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <al.h>
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
		Buffers(const Mesh& mesh, GLenum shape_type = GL_TRIANGLES);
		inline void bind() const { glBindVertexArray(VAO); };
		inline void unBind() const { glBindVertexArray(0); };
		inline void draw() const { glDrawElements(shape_type, num_indices, GL_UNSIGNED_INT, 0); };
	};
	class StaticBuffer {
	private:
		unsigned UBO;
		short bindingPoint;
		static short usedBindingPoint;
	public:
		inline StaticBuffer() : UBO(0), bindingPoint(-1) { };
		const unsigned short init(unsigned dataSize, short bindingPoint = -1);
		inline const short getBindingPoint() const { return bindingPoint; };
		void fill(unsigned offset, unsigned size, const void* data) const;
		inline void bind() const { glBindBuffer(GL_UNIFORM_BUFFER, UBO); };
	};
	class FrameBuffer {
	private:
		unsigned FBO;
		glm::ivec2 dimentions;
		std::unordered_map<std::string, unsigned> textures;
		std::vector<unsigned> renderBuffers;
	public:
		inline FrameBuffer() : textures(), FBO(0) { };
		FrameBuffer(std::vector<std::string> textuers, glm::ivec2 dimentions);
		inline void bind() const 
		{ 
			glViewport(0, 0, dimentions.x, dimentions.y);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		};
		inline void unBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); };
		inline void cleanUp() const {
			if (FBO == 0) return;
			this->unBind();
			glDeleteFramebuffers(1, &FBO);
		};
		const unsigned& getTextureId(const std::string& name);
	};
	class SoundBuffer {
	private:
		unsigned SBO; // sound buffer object
		std::string name;
		float bitDepth, sampleRate, dataSize;
	public:
		inline SoundBuffer() : SBO(0), name(), bitDepth(0), sampleRate(0), dataSize() { };
		SoundBuffer(const char* soundData, const int& len, const unsigned& channels, const int& sampleRate, const unsigned& bitDepth);
		void cleanUp();
		inline const unsigned& getSBO() const { return SBO; };
	};
}

