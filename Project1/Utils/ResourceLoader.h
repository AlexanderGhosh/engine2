#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <GL/glew.h>
#include "General.h"
#include "../Primatives/Buffers.h"
enum class TextureType {
	NormalMap, SpecularMap, DiffuseMap, HeightMap,
	AlbedoMap, MetalicMap, AOMap, RoughnessMap
};
class ResourceLoader
{
public:
	static std::string createShader(const std::string& filePath);
	static const unsigned getShader(const std::string& name);

	static const unsigned createTexture(const std::string& filePath, const TextureType type, const bool& flip = 1);
	static const unsigned getTexture(const std::string& name);

	static const std::vector<Primative::VertexBuffer*> createModel(const std::string& filePath, GLenum draw_type = GL_TRIANGLES);
	static const std::vector<Primative::VertexBuffer*> createModel(Primative::Mesh* meshes, const std::string& name, GLenum draw_type = GL_TRIANGLES);
	static const std::vector<Primative::VertexBuffer*> createModel(std::vector<Primative::Mesh*>& meshes, const std::string& name, GLenum draw_type = GL_TRIANGLES);
	static const std::vector<Primative::VertexBuffer*> getModel(const std::string& name);

	static void cleanUp();

private:
	static std::unordered_map<std::string, unsigned> shaders;
	static std::unordered_map<std::string, unsigned> textures;
	static std::list<Primative::VertexBuffer> buffers;
	static std::unordered_map<std::string, std::vector<unsigned>> models;
	static std::string defaultShaderName;
	static const std::vector<Primative::VertexBuffer*> processMeshData(std::vector<Primative::Mesh*>& data, const std::string& name, GLenum draw_type);
};

