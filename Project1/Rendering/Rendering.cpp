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

	for (const Primative::Buffers& buffer : buffers) {
		buffer.bind();
		buffer.draw();
	}
}

void Render::RenderMesh::addMesh(Primative::Mesh* m)
{
	buffers.emplace_back(*m);
};
