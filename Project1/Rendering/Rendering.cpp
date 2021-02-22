#include "Rendering.h"

#include "Shading/Manager.h"
#include "../Componets/Componets.h"
#include "../GameObject/GameObject.h"
#include "../Componets/Animated.h"

Component::RenderMesh::RenderMesh() : model(), materials(), animatedComponet(nullptr), Component::ComponetBase()
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


void Component::RenderMesh::update()
{
	glm::mat4 m(1);
	if(parent)
		m = Component::ComponetBase::parent->getTransform()->getModel();
	Render::Shading::Manager::setValue("model", m);
	/*if (materials.size() == 1) {
		Materials::Material* material = materials[0];
		Render::Shading::Manager::setValue("material", material);
		material->activateTextures();
	}*/
	if (animatedComponet) {
		const Render::Animation::KeyFrame& frame = animatedComponet->getCurrentFrame();
		Render::Shading::Manager::setValue("bones", frame);
	}

	const auto& buffers = model.getBuffers();
	for (short i = 0; i < buffers.size(); i++) {
		Primative::VertexBuffer& buffer = ResourceLoader::getBuffer(buffers[i]);
		if (materials.size() > i) {
			Materials::Material* material = materials[i % materials.size()];
			Render::Shading::Manager::setValue("material", material);
			material->activateTextures();
		}
		buffer.bind();
		buffer.draw();
		buffer.unBind();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
void Component::RenderMesh::setModel(const Primative::Model model, const GLenum draw_type)
{
	this->model = model;
	/*for (unsigned& index : model) {
		auto buffer = ResourceLoader::getBuffer(index);
		buffer->setDrawType(draw_type);
	}*/
}
void Component::RenderMesh::setAnimatedComp(Component::Animated* comp)
{
	animatedComponet = comp;
}
void Component::RenderMesh::cleanUp() {
	// Component = nullptr;
	for (auto itt = materials.begin(); itt != materials.end();) {
		itt = materials.erase(itt);
	}
}

const Primative::Model& Component::RenderMesh::getModel() const
{
	return model;
}

Component::RenderMesh Component::RenderMesh::copy() const
{
	RenderMesh res = RenderMesh();
	res.materials = materials;
	res.model = model;
	return res;
}

