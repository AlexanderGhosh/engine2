#include "SkyBox.h"
#include "../Utils/ResourceLoader.h"
#include "../Rendering/Shading/Manager.h"
#include "../Rendering/Rendering.h"
#include "../GameObject/GameObject.h"
#include "../Primatives/Mesh.h"
#include "../Primatives/Vertex.h"

unsigned SkyBox::shaderId = 0;
SkyBox::SkyBox() : texId(0), mesh(nullptr)
{
	if (!shaderId) {
		ResourceLoader::createShader("Resources/Shaders/SkyBoxShader");
		shaderId = ResourceLoader::getShader("SkyBoxShader");
	}
	if (!mesh) {
		mesh = DBG_NEW Component::RenderMesh();
		// auto model = ResourceLoader::getModel("cube.obj");
		auto m = ResourceLoader::getModel("cube.obj");
		mesh->setModel(m);
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
	mesh->render(0);
}

void SkyBox::cleanUp()
{
    if (mesh) {
        mesh->cleanUp();
        delete mesh;
        mesh = nullptr;
    }
}

void SkyBox::setTexture(const int& texId)
{
	this->texId = texId;
}

void SkyBox::setTexture(const std::string& name)
{
	texId = ResourceLoader::getTexture(name);
}


