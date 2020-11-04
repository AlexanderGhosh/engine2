#include "GameScene.h"
#include "../Rendering/Shading/Manager.h"

void GameScene::preProcess()
{
	for (const auto& layer : preProcessingLayers) {
		const std::string& name = layer.first; // of the layer
		const unsigned& shaderId = layer.second;
		const auto& fbo = getFBO(name);
		fbo->bind();
		clearFBO();
		Render::Shading::Manager::setActive(shaderId);

		if (name == "shadows") {
			glCullFace(GL_FRONT);
		}

		renderObjects();
		glCullFace(GL_BACK);
	}
}

void GameScene::postProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // default fbo
	clearFBO();
	Render::Shading::Manager::setActive(postProcShaderId);
	Render::Shading::Manager::setValue("depthMap", 3); // depth map
	/*glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE, FBOs["shadows"]->getTextureId("depth"));*/

	renderObjects();
}

void GameScene::renderObjects()
{
	for (GameObject* obj : objects) {
		obj->tick(currentTick++ % FIXED_UPDATE_RATE); // should just draw not tick
	}
}

const Primative::FrameBuffer* GameScene::getFBO(const std::string& name)
{
	if (name == "any") {
		return (*FBOs.begin()).second;
	}
	const unsigned& s = FBOs.size();
	const auto& r = FBOs[name];
	if (s < FBOs.size()) {
		FBOs.erase(name);
		return nullptr;
	}
	return r;
}

void GameScene::cleanUp()
{
	for (auto& obj : objects) {
		obj->cleanUp();
		delete obj;
		obj = nullptr;
	}
}

void GameScene::addPreProcLayer(const std::string& name, const unsigned& shaderId)
{
	this->preProcessingLayers[name] = shaderId;	
}