#pragma once
#include <glm.hpp>
#include <string>
#include "../Utils/General.h"

namespace Component {
	class RenderMesh;
}
class SkyBox
{
private:
	static unsigned shaderId;
	int texId;
	Component::RenderMesh* mesh;
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

	// getters
	Int getTextureId() const;
};

