#include "GameScene.h"
#include "../Rendering/Shading/Manager.h"
#include "SkyBox.h"
#include  "../Context.h"
#include "../Utils/ResourceLoader.h"
#include "../Rendering/Rendering.h"
#include "../Componets/Camera.h"
#include "../Primatives/Mesh.h"
#include "../Primatives/Vertex.h"

GameScene::GameScene() : objects(), preProcessingLayers(), currentTick(0), postProcShaderId(0), FBOs(), backgroundColour(0), skybox(nullptr), mainContext(nullptr), mainCamera(nullptr), opaque(), transparent(), quadBuffer(), quadFBO()
{
	Primative::Vertex v1 = Primative::Vertex({ 0, 0, 0 });
	Primative::Vertex v2 = Primative::Vertex({ 1, 0, 0 });
	Primative::Vertex v3 = Primative::Vertex({ 0, 1, 0 });
	Primative::Vertex v4 = Primative::Vertex({ 1, 1, 0 });
	Primative::Mesh m = Primative::Mesh({ v1, v2, v3, v4 }, { 0, 1, 2, 3 }, "ScreenQuad");
	quadBuffer = Primative::VertexBuffer(m, GL_TRIANGLE_STRIP);
	addPreProcLayer("MainScene", ResourceLoader::getShader("PBRShader"));
	quadFBO = DBG_NEW Primative::FrameBuffer({ "col" }, { 800, 600 });
	addFBO("MainScene", quadFBO);
}

void GameScene::addObject(GameObject* obj)
{
	objects.push_back(obj);
	Component::RenderMesh* comp = obj->getComponet<Component::RenderMesh>();
	if (comp) {
		if (comp->getTransparent()) {
			transparent[rand()] = comp;
		}
		else {
			opaque.push_back(comp);
		}
	}
};

void GameScene::preProcess()
{
	for (const auto& layer : preProcessingLayers)
	{

		const std::string& name = layer.first; // of the layer
		const unsigned& shaderId = layer.second;
		const auto& fbo = getFBO(name);
		fbo->bind();
		fbo->clear();
		Render::Shading::Manager::setActive(shaderId);
		if (name == "MainScene") {
			drawOpaque();
			drawTransparent();
		}
	}
}

void GameScene::postProcess()
{
	Render::Shading::Manager::setActive(postProcShaderId);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // default fbo
	clearFBO();
	glBindTexture(GL_TEXTURE_2D, FBOs["MainScene"]->getTextureId("col0"));
	quadBuffer.bind();
	quadBuffer.draw();
	quadBuffer.unBind();
}

void GameScene::updateScene()
{
	for (GameObject*& obj : objects) {
		obj->tick(currentTick++ % FIXED_UPDATE_RATE, mainContext->getTime().deltaTime);
	}
}

void GameScene::renderObjects()
{
	for (GameObject*& obj : objects) {
		obj->tryDraw(mainContext->getTime().deltaTime);
	}
}

void GameScene::renderSkyBox()
{
	if (!skybox)
		return;
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skybox->draw();
	glDepthFunc(GL_LESS); // set depth function back to default
	glEnable(GL_CULL_FACE);
}

void GameScene::drawOpaque()
{
	for (Component::RenderMesh* mesh : opaque) {
		mesh->update(mainContext->getTime().deltaTime);
	}
	renderSkyBox();
}
void GameScene::drawTransparent()
{
	if (NOT transparent.size()) {
		return;
	}
	std::map<float, Component::RenderMesh*> sortedTransparents;
	for (auto itt = transparent.rbegin(); itt != transparent.rend(); itt++) {
		Component::RenderMesh* mesh = (*itt).second;
		mesh->update(mainContext->getTime().deltaTime);
		sortedTransparents[glm::length2(mainCamera->getPos() - mesh->getParent()->getTransform()->Position)] = mesh;
	}
	transparent.clear();
	transparent = sortedTransparents;
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
	mainContext->cleanUp();
	skybox = nullptr;
	mainCamera = nullptr;
	opaque.clear();
	transparent.clear();
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

void GameScene::setMainCamera(Component::Camera* camera)
{
	mainCamera = camera;
}
