#include "Rendering.h"

#include "Shading/Manager.h"
#include "../Componets/Componets.h"
#include "../GameObject/GameObject.h"

Render::RenderMesh::RenderMesh() : buffers(), materials(), Component::ComponetBase()
{
	/*Materials::Forward* fwd = DBG_NEW Materials::Forward();
	fwd->getDiffuse()(1); // set to the texture id
	// fwd.getDiffuse()({ 1, 0, 0 });
	fwd->getSpecular()(2);
	fwd->getNormals()(3);
	fwd->shininess = 32;
	this->material = fwd;

	Materials::PBR* prb = DBG_NEW Materials::PBR();
	prb->getAlbedo()(1);
	prb->getAO()({ 1, 1, 1 });
	prb->getNormal()(3);
	prb->getMetalic()(2); // { 1.0f/7.0f, 0, 0 }
	prb->getRoughness()({ 0.025, 0.1, 0.1 });
	this->material = prb;*/
}


void Render::RenderMesh::update()
{
	glm::mat4 m(1);
	if(parent)
		m = Component::ComponetBase::parent->getTransform()->getModel();
	Shading::Manager::setValue("model", m);
	if (materials.size() == 1) {
		Materials::Material* material = materials[0];
		Shading::Manager::setValue("material", material);
		material->activateTextures();
	}
	for (short i = 0; i < buffers.size(); i++) {
		Primative::VertexBuffer& buffer = ResourceLoader::getBuffer(buffers[i]);
		if (materials.size() > 1) {
			Materials::Material* material = materials[i % materials.size()];
			Shading::Manager::setValue("material", material);
			material->activateTextures();
		}
		buffer.bind();
		buffer.draw();
		buffer.unBind();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
void Render::RenderMesh::addBuffers(std::vector<unsigned>& buffers, const GLenum draw_type)
{
	this->buffers = buffers;
	/*for (unsigned& index : buffers) {
		auto buffer = ResourceLoader::getBuffer(index);
		buffer->setDrawType(draw_type);
	}*/
}
void Render::RenderMesh::cleanUp() {
	parent = nullptr;
	for (auto itt = buffers.begin(); itt != buffers.end();) {
		itt = buffers.erase(itt);
	}
	for (auto itt = materials.begin(); itt != materials.end();) {
		itt = materials.erase(itt);
	}
}

