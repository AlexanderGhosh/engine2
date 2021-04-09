#include "Rendering.h"

#include "Shading/Manager.h"
#include "Animation/Animation.h"
#include "../Componets/Animated.h"
#include "../Utils/ResourceLoader.h"
#include "../Primatives/Buffers/VertexBuffer.h"
#include "../GameObject/GameObject.h"
#include "../Primatives/Materials/MaterialBase.h"

Component::RenderMesh::RenderMesh() : model(), materials(), animatedComponet(nullptr), Component::ComponetBase(), isTransparent(false)
{
}

void Component::RenderMesh::update(float deltaTime)
{
	glm::mat4 m(1);
	if(parent)
		m = parent->getTransform()->getModel();
	Render::Shading::Manager::setValue("model", m);
	/*if (materials.size() == 1) {
		Materials::Material* material = materials[0];
		Render::Shading::Manager::setValue("material", material);
		material->activateTextures();
	}*/
	if (animatedComponet) {
		const Render::Animation::KeyFrame frame = animatedComponet->getCurrentFrame();
		// Render::Shading::Manager::setValue("bones", frame);
		animatedComponet->bind();
	}

	const auto& buffers = model.getBuffers();
	for (short i = 0; i < buffers.size(); i++) {
		Primative::Buffers::VertexBuffer& buffer = ResourceLoader::getBuffer(buffers[i]);
		const Materials::MaterialBase* material = materials[i];
		if (material) {
			material->activateTextures(); // activates the texture units and binds the ids
			Render::Shading::Manager::setValue("material", material); // sets values to the samplers
		}
		buffer.render();
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
void Component::RenderMesh::setMaterial(Materials::MaterialBase* material)
{
	for (unsigned i = 0; i < materials.size(); i++) {
		materials[i] = material;
	}
}
void Component::RenderMesh::setMaterialTo(Materials::MaterialBase* material, String meshName)
{
	unsigned i = 0;
	for (Unsigned buff : model.getBuffers()) {
		const Primative::Buffers::VertexBuffer& vertexBuffer = ResourceLoader::getBuffer(buff);
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

