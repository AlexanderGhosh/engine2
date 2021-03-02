#include "GameScene.h"
#include "../Rendering/Shading/Manager.h"
#include "SkyBox.h"
#include  "../Context.h"
#include "../Rendering/Rendering.h"
#include "../Componets/Camera.h"

GameScene::GameScene() : objects(), preProcessingLayers(), currentTick(0), postProcShaderId(0), FBOs(), backgroundColour(0), skybox(nullptr), mainContext(nullptr), opaque(), transparent()
{

}

void GameScene::drawOpaque()
{
	for (Component::RenderMesh* mesh : opaque) {
		mesh->update(mainContext->getTime().deltaTime);
	}
}

void GameScene::drawTransparent()
{
	if (NOT transparent.size()) {
		return;
	}
	std::map<float, Component::RenderMesh*> sorted;
	for (auto itt = transparent.rbegin(); itt != transparent.rend(); itt++) {
		float dist = (*itt).first;
		Component::RenderMesh* mesh = (*itt).second;
		mesh->update(mainContext->getTime().deltaTime);
		dist = glm::length2(mesh->getParent()->getTransform()->Position - mainCamera->getPos());
		sorted[dist] = mesh;
	}
	transparent.clear();
	transparent = sorted;
}

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

		drawObjects();
		drawSkyBox();
		glCullFace(GL_BACK);
	}
}

void GameScene::postProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // default fbo
	clearFBO();
	Render::Shading::Manager::setActive(postProcShaderId);
	//Render::Shading::Manager::setValue("depthMap", 3); // depth map
	/*glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE, FBOs["shadows"]->getTextureId("depth"));*/

	drawObjects();
	glDisable(GL_CULL_FACE);
	drawSkyBox();
	glEnable(GL_CULL_FACE);
}

void GameScene::updateScene()
{
	for (GameObject*& obj : objects) {
		obj->tick(currentTick++ % FIXED_UPDATE_RATE, mainContext->getTime().deltaTime);
	}
}

void GameScene::drawObjects()
{
	for (GameObject*& obj : objects) {
		obj->tryDraw(mainContext->getTime().deltaTime);
	}
}

void GameScene::drawSkyBox()
{
	if (!skybox)
		return;
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skybox->draw();
	glDepthFunc(GL_LESS); // set depth function back to default
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
		//delete obj;
		obj = nullptr;
	}
	objects.clear();
	for (auto& pair : FBOs) {
		Primative::FrameBuffer*& fbo = pair.second;
		fbo->cleanUp();
		delete fbo;
		fbo = nullptr;
	}
	FBOs.clear();
	if(skybox)
		skybox->cleanUp();
	skybox = nullptr;
}

void GameScene::addPreProcLayer(const std::string& name, const unsigned& shaderId)
{
	this->preProcessingLayers[name] = shaderId;	
}

void GameScene::setSkyBox(SkyBox* sb)
{
	skybox = sb;
}

void GameScene::setContext(Context* context)
{
	mainContext = context;
}
