#include "SkyBox.h"
#include "../Utils/ResourceLoader.h"
#include "../Rendering/Shading/Manager.h"
#include "../Rendering/Rendering.h"

unsigned SkyBox::shaderId = 0;
SkyBox::SkyBox() : texId(0), mesh(nullptr)
{
	if (!shaderId) {
		ResourceLoader::createShader("Basics/SkyBoxShader");
		shaderId = ResourceLoader::getShader("SkyBoxShader");
	}
	if (!mesh) {
		mesh = DBG_NEW Render::RenderMesh();
        Primative::Mesh sbMesh = Primative::Mesh({
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),

            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),

            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),

            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f,  1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),

            glm::vec3(-1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f, -1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3( 1.0f,  1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f,  1.0f),
            glm::vec3(-1.0f,  1.0f, -1.0f),

            glm::vec3(-1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3( 1.0f, -1.0f, -1.0f),
            glm::vec3(-1.0f, -1.0f,  1.0f),
            glm::vec3( 1.0f, -1.0f,  1.0f)
            }
		);
        for (short i = 0; i < sbMesh.verts.size(); i++) {
            sbMesh.indices.push_back(i);
        }
		auto model = ResourceLoader::createModel(&sbMesh, "skybox", GL_TRIANGLES, false);
		// auto model = ResourceLoader::getModel("cube.obj");
		mesh->addBuffers(model);
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
	bool t = Render::Shading::Manager::setValue("skybox", 0);
	mesh->update();
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


