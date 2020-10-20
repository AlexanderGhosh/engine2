#pragma once
#include <unordered_map>
#include <string>
#include <GL/glew.h>
#include "General.h"
enum class TextureType {
	NormalMap, SpecularMap, DiffuseMap, HeightMap
};
class ResourceLoader
{
public:
	static std::string createShader(const std::string& filePath);
	static const unsigned getShader(const std::string& name);

	static const unsigned createTexture(const std::string& filePath, const TextureType type);
	static void cleanUp();
private:
	static std::unordered_map<std::string, unsigned> shaders;
};

