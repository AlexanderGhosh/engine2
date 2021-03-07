#include "SkyBox.h"
#include "../Utils/ResourceLoader.h"
#include "../Rendering/Shading/Manager.h"
#include "../Primatives/Model.h"
#include "../Primatives/Buffers/VertexBuffer.h"

unsigned SkyBox::shaderId = 0;
Primative::Model SkyBox::model = { };
SkyBox::SkyBox() : texId(0)
{
	if (!shaderId) {
		shaderId = ResourceLoader::getShader("SkyBoxShader");
	}
	
	if (!model.hasMesh()) {
		model = ResourceLoader::getModel("cube.obj");
	}
}

SkyBox::SkyBox(const int& texId) : SkyBox()
{
	setTexture(texId);
}

SkyBox::SkyBox(const std::string& name) : SkyBox()
{
	setTexture(name);
}

void SkyBox::draw()
{
	Render::Shading::Manager::setActive(shaderId);
	Render::Shading::Manager::setValue("skybox", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
	ResourceLoader::getBuffer(model.getBuffers()[0]).render();
}

void SkyBox::cleanUp()
{
	model.cleanUp();
}

void SkyBox::setTexture(const int& texId)
{
	this->texId = texId;
}

void SkyBox::setTexture(const std::string& name)
{
	texId = ResourceLoader::getTexture(name);
}


