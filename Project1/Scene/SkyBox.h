#pragma once
#include <glm.hpp>
#include <string>
#include "../Primatives/Material.h"

namespace Primative {
	class Model;
}
class SkyBox
{
private:
	static unsigned shaderId;
	int texId;
	static Primative::Model model;
public:
	SkyBox();
	SkyBox(const int& texId);
	SkyBox(const std::string& name);
	~SkyBox() = default;

	void draw();
	void cleanUp();

	// setters
	void setTexture(const int& texId);
	void setTexture(const std::string& name);
};

