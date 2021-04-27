#include "GUIBaseShape.h"
#include "../Primatives/Materials/MatItemBase.h"
#include "../Rendering/Shading/Manager.h"
#include "../Utils/ResourceLoader.h"
#include "../Primatives/Model.h"

Primative::Buffers::VertexBuffer GUI::GUIBaseShape::quadBuffer{};
unsigned GUI::GUIBaseShape::shaderId = 0;

GUI::GUIBaseShape::GUIBaseShape() : albedo(nullptr), radius(0)
{
	if (NOT shaderId) {
		quadBuffer = ResourceLoader::getBuffer(ResourceLoader::getModel("plane.dae").getBuffers()[0]);
		shaderId = ResourceLoader::getShader("BaseShapeShader");
	}
}

void GUI::GUIBaseShape::cleanUp()
{
	if(albedo)
		albedo->cleanUp();
}

void GUI::GUIBaseShape::setAlbedo(Materials::MatItemBase<glm::vec4>* albedo)
{
	this->albedo = albedo;
}

void GUI::GUIBaseShape::setRadius(Float r)
{
	radius = r;
}

void GUI::GUIBaseShape::render(Matrix4 model, Vector2 dimetions, Vector2 parentDim)
{
	Render::Shading::Manager::setActive(shaderId);
	Render::Shading::Manager::setValue("model", model);
	Render::Shading::Manager::setValue("dimentions", dimetions);
	Render::Shading::Manager::setValue("radius", radius);
	Render::Shading::Manager::setValue("parentDimentions", parentDim);
	if (albedo) {
		int unit = 0;
		albedo->tryBindTexture(unit);
		Render::Shading::Manager::setValue("albedo", *albedo, unit);
	}
	quadBuffer.render();
}
