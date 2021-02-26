#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <glm.hpp>
#include <GL/glew.h>
#include "General.h"
#include "../Rendering/Animation/Animation.h"
namespace Materials {
	class Material;
	struct PBR;
	struct Forward;
}
namespace Primative {
	class VertexBuffer;
	struct Mesh;
	class Model;
}
enum class TextureType {
	NormalMap, SpecularMap, DiffuseMap, HeightMap,
	AlbedoMap, MetalicMap, AOMap, RoughnessMap,
	CubeMap
};
class ResourceLoader
{
public:
	// shaders
	static std::string createShader(const std::string& filePath, bool hasGeom = false);
	static const unsigned getShader(const std::string& name);
	// materials
	static const Materials::PBR createPBR(String dirPath, std::vector<TextureType> types, std::vector<bool> flip);

	// textures
	static const std::vector<unsigned> loadTextureFile(String dirPath, std::vector<TextureType> types, std::vector<bool> flip);
	static const unsigned loadTexture(const std::string& filePath, const TextureType type, const bool& flip = 1);
	static const unsigned loadCubeMap(const std::string& dirPath, const std::string& extension, const bool& flip = 1);
	static const unsigned getTexture(const std::string& name);
	// models
	static const Primative::Model createModel(const std::string& filePath, GLenum draw_type = GL_TRIANGLES);
	static const Primative::Model createModel(Primative::Mesh* meshes, const std::string& name, GLenum draw_type = GL_TRIANGLES, const bool deleteAble = true);
	static const Primative::Model createModel(std::vector<Primative::Mesh*>& meshes, const std::string& name, GLenum draw_type = GL_TRIANGLES, const bool deleteAble = true);
	static const Primative::Model getModel(const std::string& name);
	static Primative::VertexBuffer& getBuffer(const unsigned& index);
	static Materials::Material* getMaterial(const int& index);
	static void addMaterial(Materials::Material* mat);
	// animation
	static const Render::Animation::Animation& createAnimation(String filePath);
	static Render::Animation::Animation* getAnimation(String name);
	// general
	static void cleanUp();
private:
	static std::unordered_map<std::string, unsigned> shaders;
	static std::unordered_map<std::string, unsigned> textures;
	static std::vector<Primative::VertexBuffer> buffers;
	static std::unordered_map<std::string, Primative::Model> models;
	static std::unordered_map<std::string, Render::Animation::Animation> animations;
	static std::string defaultShaderName;
	static std::vector<Materials::Material*> materials;
	static const Primative::Model processMeshData(std::vector<Primative::Mesh*>& data, const std::string& name, GLenum draw_type, const bool deleteAble);
};