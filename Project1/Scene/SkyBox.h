#pragma once
#include "../GameObject/GameObject.h"
namespace Render {
	class RenderMesh;
}
class SkyBox
{
private:
	static unsigned shaderId;
	int texId;
	Render::RenderMesh* mesh;
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

