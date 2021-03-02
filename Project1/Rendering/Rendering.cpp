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


void Component::RenderMesh::update(float deltaTime)
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
		const Render::Animation::KeyFrame frame = animatedComponet->getCurrentFrame();
		Render::Shading::Manager::setValue("bones", frame);
		// animatedComponet->bind();
	}

	const auto& buffers = model.getBuffers();
	for (short i = 0; i < buffers.size(); i++) {
		Primative::VertexBuffer& buffer = ResourceLoader::getBuffer(buffers[i]);
		const Materials::Material* material = materials[i];
		if (material) {
			Render::Shading::Manager::setValue("material", material); // sets values to the samplers
			material->activateTextures(); // activates the texture units and binds the ids
		}
		buffer.bind();
		buffer.draw();
		buffer.unBind();
	}
	if (animatedComponet)
		animatedComponet->unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
void Component::RenderMesh::setModel(const Primative::Model model, const GLenum draw_type)
{
	this->model = model;
	materials.reserve(model.getBuffers().size());
	materials.resize(model.getBuffers().size());
	/*for (unsigned& index : model) {
		auto buffer = ResourceLoader::getBuffer(index);
		buffer->setDrawType(draw_type);
	}*/
}
void Component::RenderMesh::setMaterial(Materials::Material* material)
{
	for (unsigned i = 0; i < materials.size(); i++) {
		materials[i] = material;
	}
}
void Component::RenderMesh::setMaterialTo(Materials::Material* material, String meshName)
{
	unsigned i = 0;
	for (Unsigned buff : model.getBuffers()) {
		const Primative::VertexBuffer& vertexBuffer = ResourceLoader::getBuffer(buff);
		if (vertexBuffer.getName() == meshName) {
			materials[i] = material;
			return;
		}
		i++;
	}
}
void Component::RenderMesh::setTransparent(bool transparent)
{
	this->isTransparent = transparent;
}
const bool Component::RenderMesh::getTransparent() const
{
	return isTransparent;
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

