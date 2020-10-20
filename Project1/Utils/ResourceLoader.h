#pragma once
#include <unordered_map>
#include <string>
#include <GL/glew.h>
#include "General.h"
class ResourceLoader
{
public:
	static std::string createShader(const std::string& filePath);
	static const unsigned getShader(const std::string& name);
	static void cleanUp();
private:
	static std::unordered_map<std::string, unsigned> shaders;
};

