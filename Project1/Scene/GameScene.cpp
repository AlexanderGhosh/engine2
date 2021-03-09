#include "GameScene.h"
#include "../Rendering/Shading/Manager.h"
#include "SkyBox.h"
#include  "../Context.h"
#include "../Rendering/Rendering.h"
#include "../Componets/Camera.h"
#include "../GameObject/Terrain.h"
#include "../GameObject/GameObject.h"
#include "../Utils/ResourceLoader.h"
#include "../Primatives/Buffers/FrameBuffer.h"
#include "../Primatives/Buffers/VertexBuffer.h"

GameScene::GameScene() : objects(), preProcessingLayers(), currentTick(0), postProcShaderId(0), FBOs(), backgroundColour(0),
							skybox(nullptr), mainContext(nullptr), opaque(), transparent(), mainCamera(nullptr), terrain(), quadModel()
{
	quadModel = ResourceLoader::getModel("plane.dae");
}

void GameScene::drawOpaque()
{
	for (Component::RenderMesh* mesh : opaque) {
		if(mesh->getParent()->isAlive())
			mesh->update(mainContext->getTime().deltaTime);
	}
}

void GameScene::drawTransparent()
{
	Render::Shading::Manager::setActive(ResourceLoader::getShader("TransparentShader"));
	assert(mainCamera);
	if (NOT transparent.size()) {
		return;
	}
	mainContext->enable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::map<float, Component::RenderMesh*> sorted;
	for (auto itt = transparent.rbegin(); itt != transparent.rend(); itt++) {
		float dist = (*itt).first;
		Component::RenderMesh* mesh = (*itt).second;
		if (mesh->getParent()->isAlive())
			mesh->update(mainContext->getTime().deltaTime);
		dist = glm::length2(mesh->getParent()->getTransform()->Position - mainCamera->getPos());
		sorted[dist] = mesh;
	}
	transparent.clear();
	transparent = sorted;
}

void GameScene::drawTerrain()
{
	for (Terrain* terrain : this->terrain) {
		terrain->draw();
	}
}

void GameScene::addObject(GameObject* obj)
{
	objects.push_back(obj);
	Component::RenderMesh* mesh = obj->getComponet<Component::RenderMesh>();
	if (mesh) {
		if (mesh->getTransparent()) {
			transparent[rand()] = mesh;
		}
		else {
			opaque.push_back(mesh);
		}
	}
}

void GameScene::addTerrain(Terrain* terrain)
{
	this->terrain.push_back(terrain);
}

void GameScene::setBG(Vector3 col) 
{ 
	backgroundColour = col; 
};

void GameScene::preProcess()
{
	for (const auto& layer : preProcessingLayers) {
		const std::string& name = layer.first;
		const unsigned& shaderId = layer.second;
		const auto& fbo = getFBO(name);
		fbo->bind();
		fbo->clearBits();
		Render::Shading::Manager::setActive(shaderId);

		if (name == "shadows") {
			glCullFace(GL_FRONT);
			drawOpaque();
			glCullFace(GL_BACK);
		}
		else {
			drawSkyBox();
			drawObjects(shaderId);
		}
		fbo->unBind();
	}
}

void GameScene::postProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // default fbo
	clearFBO();


	Render::Shading::Manager::setActive(postProcShaderId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOs["final"]->getTexture("col0"));
	Render::Shading::Manager::setValue("tex", 0);

	const Primative::Buffers::VertexBuffer& buffer = ResourceLoader::getBuffer(quadModel.getBuffers()[0]);
	//drawSkyBox();
	buffer.render();


	// drawSkyBox();
	// drawObjects(postProcShaderId);
}

void GameScene::updateScene()
{
	for (GameObject*& obj : objects) {
		if(obj->isAlive())
			obj->tick(currentTick++ % FIXED_UPDATE_RATE, mainContext->getTime().deltaTime);
	}
}

void GameScene::clearFBO() const 
{
	glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GameScene::drawObjects(Unsigned shaderId)
{
	Render::Shading::Manager::setActive(shaderId);
	drawOpaque();
	drawTerrain();
	Render::Shading::Manager::setActive(shaderId);
	drawTransparent();
}

void GameScene::drawSkyBox()
{
	if (!skybox)
		return;
	mainContext->disable(GL_DEPTH_TEST);
	mainContext->disable(GL_CULL_FACE);
	//glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skybox->draw();
	//glDepthFunc(GL_LESS); // set depth function back to default
	mainContext->enable(GL_CULL_FACE);
	mainContext->enable(GL_DEPTH_TEST);
}

const Primative::Buffers::FrameBuffer* GameScene::getFBO(const std::string& name)
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
		Primative::Buffers::FrameBuffer*& fbo = pair.second;
		fbo->cleanUp();
		//delete fbo;
		fbo = nullptr;
	}
	FBOs.clear();
	if(skybox)
		skybox->cleanUp();
	skybox = nullptr;
	mainCamera = nullptr;
	opaque.clear();
	transparent.clear();
}

void GameScene::addPreProcLayer(const std::string& name, const unsigned& shaderId)
{
	this->preProcessingLayers[name] = shaderId;	
}

void GameScene::addFBO(const std::string& layerName, Primative::Buffers::FrameBuffer* fbo)
{
	FBOs.insert({ layerName, fbo });
};

void GameScene::setPostProcShader(const unsigned& shaderId)
{
	postProcShaderId = shaderId;
};

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
