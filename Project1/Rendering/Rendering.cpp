#include "Rendering.h"

#include "Shading/Manager.h"
#include "../Componets/Componets.h"
#include "../GameObject/GameObject.h"

Render::RenderMesh::RenderMesh() : buffers(), shaderName("default"), parent(nullptr), shaderId(ResourceLoader::getShader(shaderName)), material(), Componet::Base()
{
	Materials::Forward fwd; 
	fwd.getDiffuse()(1); // set to the texture id
	// fwd.getDiffuse()({ 1, 0, 0 }); // set to the texture id
	fwd.getSpecular()(2);
	fwd.getNormals()(3);
	fwd.shininess = 32;
	this->material = fwd;
}


void Render::RenderMesh::update()
{
	glUseProgram(shaderId);
	Shading::Manager::setActive(shaderId);
	const glm::mat4 m = Componet::Base::parent->getTransform()->getModel();
	Shading::Manager::setValue("model", m);

	Shading::Manager::setValue("material", this->material);
	// Shading::Manager::setValue("t", 0);

	this->material.activateTextures();

	for (const Primative::Buffers& buffer : buffers) {
		buffer.bind();
		buffer.draw();
	}
}

void Render::RenderMesh::addMesh(Primative::Mesh* m)
{
	buffers.emplace_back(*m);
};
