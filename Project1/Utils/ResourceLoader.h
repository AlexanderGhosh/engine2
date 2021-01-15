#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <glm.hpp>
#include <GL/glew.h>
#include "General.h"
namespace Materials {
	class Material;
}
namespace Primative {
	class VertexBuffer;
	struct Mesh;
}
enum class TextureType {
	NormalMap, SpecularMap, DiffuseMap, HeightMap,
	AlbedoMap, MetalicMap, AOMap, RoughnessMap,
	CubeMap
};
class ResourceLoader
{
public:
	static std::string createShader(const std::string& filePath);
	static const unsigned getShader(const std::string& name);

	static const unsigned createTexture(const std::string& filePath, const TextureType type, const bool& flip = 1);
	static const unsigned createCubeMap(const std::string& dirPath, const std::string& extension, const bool& flip = 1);
	static const unsigned getTexture(const std::string& name);

	static const std::vector<unsigned> createModel(const std::string& filePath, GLenum draw_type = GL_TRIANGLES);
	static const std::vector<unsigned> createModel(Primative::Mesh* meshes, const std::string& name, GLenum draw_type = GL_TRIANGLES, const bool deleteAble = true);
	static const std::vector<unsigned> createModel(std::vector<Primative::Mesh*>& meshes, const std::string& name, GLenum draw_type = GL_TRIANGLES, const bool deleteAble = true);
	static const std::vector<unsigned> getModel(const std::string& name);
	static Primative::VertexBuffer& getBuffer(const unsigned& index);
	static Materials::Material* getMaterial(const int& index);
	static void addMaterial(Materials::Material* mat);
	static void cleanUp();

private:
	static std::unordered_map<std::string, unsigned> shaders;
	static std::unordered_map<std::string, unsigned> textures;
	static std::vector<Primative::VertexBuffer> buffers;
	static std::unordered_map<std::string, std::vector<unsigned>> models;
	static std::string defaultShaderName;
	static std::vector<Materials::Material*> materials;
	static const std::vector<unsigned> processMeshData(std::vector<Primative::Mesh*>& data, const std::string& name, GLenum draw_type, const bool deleteAble);
};