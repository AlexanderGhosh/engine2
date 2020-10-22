#include "Rendering.h"

#include "Shading/Manager.h"
#include "../Componets/Componets.h"
#include "../GameObject/GameObject.h"

Render::RenderMesh::RenderMesh() : buffers(), shaderName("default"), parent(nullptr), shaderId(ResourceLoader::getShader(shaderName)), Componet::Base()
{
}


void Render::RenderMesh::update()
{
	glUseProgram(shaderId);
	Shading::Manager::setActive(shaderId);
	const glm::mat4 m = Componet::Base::parent->getTransform()->getModel();
	Shading::Manager::setValue("model", m);

	Materials::Forward fwd;
	fwd.getDiffuse()(1); // set to the texture id
	fwd.getSpecular()({ 0.5, 0.5, 0.5 });
	fwd.shininess = 32;

	Shading::Manager::setValue("material", fwd);
	// Shading::Manager::setValue("t", 0);

	fwd.activateTextures();

	for (const Primative::Buffers& buffer : buffers) {
		buffer.bind();
		buffer.draw();
	}
}

void Render::RenderMesh::addMesh(Primative::Mesh* m)
{
	buffers.emplace_back(*m);
};
