#pragma once
#include <unordered_map>
#include <string>
#include <GL/glew.h>
using namespace std;
class ResourceLoader
{
public:
	static string createShader(const string& filePath);
	static unsigned getShader(const string& name);
private:
	static unordered_map<string, unsigned> shaders;
};

