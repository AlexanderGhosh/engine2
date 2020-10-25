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
	bool succ = Shading::Manager::setValue("model", m);

	succ = Shading::Manager::setValue("material", this->material);

	succ = Shading::Manager::setValue("depthMap", 3);
	/*glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 4);*/
	this->material.activateTextures();
	for (const Primative::Buffers& buffer : buffers) {
		buffer.bind();
		buffer.draw();
		buffer.unBind();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Render::RenderMesh::addMesh(Primative::Mesh* m, GLenum draw_type)
{
	buffers.emplace_back(*m, draw_type);
};
