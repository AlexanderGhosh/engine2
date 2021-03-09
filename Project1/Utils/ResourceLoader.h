#pragma once
#include <unordered_map>
#include <string>
#include <list>
#include <glm.hpp>
#include <GL/glew.h>

#include "General.h"
// #include "../Rendering/Animation/Animation.h"
// #include "../Rendering/Animation/Bones.h"

namespace Materials {
	class Material;
	struct PBR;
	struct Forward;
}
namespace Primative {
	namespace Buffers {
		class VertexBuffer;
	}
	struct Mesh;
	class Model;
}
namespace Render {
	namespace Animation {
		class Animation;
		class Skeleton;
	}
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
	static std::string createShader(String filePath, bool hasGeom = false);
	static const unsigned getShader(String name);
	// materials
	static Materials::PBR createPBR(String dirPath, std::vector<TextureType> types, std::vector<bool> flip);

	// textures
	static const std::vector<unsigned> loadTextureFile(String dirPath, std::vector<TextureType> types, std::vector<bool> flip);
	static const unsigned loadTexture(String filePath, const TextureType type, const bool& flip = 1);
	static const unsigned loadCubeMap(String dirPath, String extension, const bool& flip = 1);
	static const unsigned getTexture(String name);
	// models
	static const Primative::Model createModel(String filePath, GLenum draw_type = GL_TRIANGLES);
	static const Primative::Model createModel(Primative::Mesh* meshes, String name, GLenum draw_type = GL_TRIANGLES, const bool deleteAble = true);
	static const Primative::Model createModel(std::vector<Primative::Mesh*>& meshes, String name, GLenum draw_type = GL_TRIANGLES, const bool deleteAble = true);
	static const Primative::Model getModel(String name);
	static Primative::Buffers::VertexBuffer& getBuffer(Unsigned index);
	static Materials::Material* getMaterial(Int index);
	static void addMaterial(Materials::Material* mat);
	// animation
	static const Render::Animation::Animation createAnimation(String filePath, const Render::Animation::Skeleton& skeleton);
	static Render::Animation::Animation* getAnimation(String name);
	// general
	static void cleanUp();
private:
	static std::unordered_map<std::string, unsigned> shaders;
	static std::unordered_map<std::string, unsigned> textures;
	static std::vector<Primative::Buffers::VertexBuffer> buffers;
	static std::unordered_map<std::string, Primative::Model> models;
	static std::unordered_map<std::string, Render::Animation::Animation> animations;
	static std::string defaultShaderName;
	static std::vector<Materials::Material*> materials;

	static const Primative::Model processMeshData(std::vector<Primative::Mesh*>& data, String name, GLenum draw_type, const bool deleteAble);
};