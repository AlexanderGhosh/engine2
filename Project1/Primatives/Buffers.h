#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <tuple>
#include <al.h>
#include "../Utils/General.h"
namespace Primative {
	struct Mesh;
	class VertexBuffer
	{
	private:
		unsigned VBO, VAO, EBO;
		unsigned num_indices;
		GLenum shape_type, drawType;
		std::string name;
	public:
		inline const unsigned getVBO() const { return VBO; };
		inline const unsigned getVAO() const { return VAO; };
		inline const unsigned getEBO() const { return EBO; };

		inline VertexBuffer() : VBO(0), VAO(0), EBO(0), num_indices(0), shape_type(GL_TRIANGLES), drawType(GL_STATIC_DRAW), name("") { };
		VertexBuffer(/*const*/ Mesh& mesh, GLenum shape_type = GL_TRIANGLES, GLenum draw_type = GL_STATIC_DRAW, std::string name = "");
		inline void bind() const { glBindVertexArray(VAO); };
		inline void unBind() const { glBindVertexArray(0); };
		inline void draw() const { 
			glDrawElements(shape_type, num_indices, GL_UNSIGNED_INT, 0); };
		inline void setDrawType(GLenum type) { drawType = type; };
		String getName() const;
		void setName(String name);
		void cleanUp();
	};
	class StaticBuffer {
	private:
		unsigned UBO;
		char bindingPoint;
		std::vector<std::pair<unsigned, unsigned>> positions;
		static char usedBindingPoint;
		const void init(unsigned dataSize, short bindingPoint = -1);
	public:
		inline StaticBuffer() : UBO(0), bindingPoint(-1) { };
		StaticBuffer(const std::vector<std::string>& types, char bindingPoint = -1);
		StaticBuffer(const std::string& types, char bindingPoint = -1);
		inline const short getBindingPoint() const { return bindingPoint; };
		void fill(unsigned offset, unsigned size, const void* data) const;
		void fill(unsigned position, const void* data) const;
		inline void bind() const { glBindBuffer(GL_UNIFORM_BUFFER, UBO); };
		void cleanUp();
	};
	class FrameBuffer {
	protected:
		unsigned FBO;
		glm::ivec2 dimentions;
		std::unordered_map<std::string, unsigned> textures;
		std::vector<unsigned> renderBuffers;
		const std::tuple<GLenum, int, std::string> getTexType(const std::string& type, char& colTypes);
	public:
		inline FrameBuffer() : textures(), FBO(0), dimentions(0), renderBuffers() { };
		FrameBuffer(std::vector<std::string> textuers, glm::ivec2 dimentions);
		virtual inline void bind() const
		{ 
			glViewport(0, 0, dimentions.x, dimentions.y);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		};
		virtual inline void unBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); };
		virtual void cleanUp();
		inline const unsigned& getFBO() const { return FBO; };
		virtual const unsigned& getTextureId(const std::string& name);
	};
	class MSAABuffer : public FrameBuffer {
	private:
		FrameBuffer* middleMan;
	public:
		inline MSAABuffer() : FrameBuffer(), middleMan(nullptr) { };
		MSAABuffer(std::vector<std::string> textuers, glm::ivec2 dimentions);
		virtual void bind() const;
		virtual void unBind() const;
		virtual void cleanUp(); 
		virtual const unsigned& getTextureId(const std::string& name);
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

