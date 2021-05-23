#include "GameScene.h"
#include "../Rendering/Shading/Manager.h"
#include "SkyBox.h"
#include "../Rendering/Rendering.h"
#include "../Componets/Camera.h"
#include "../GameObject/Terrain.h"
#include "../GameObject/GameObject.h"
#include "../Utils/ResourceLoader.h"
#include "../Primatives/Buffers/FrameBuffer.h"
#include "../Primatives/Buffers/VertexBuffer.h"
#include "../Primatives/Buffers/UniformBuffer.h"
#include "../EventSystem/Handler.h"
#include "../Gizmos/GizmoRenderer.h"
#include "../UI/UIRenderer.h"
#include "../UI/Panes/Canvas.h"
#include "../ParticleSystem/ParticleEmmiter.h"
#include "../Componets/Lights/LightBase.h"
#include "../Componets/Lights/ShadowCaster.h"
#include "../Context.h"
// GUI
#include "../GUI/ElementContainers/GUICanvas.h"
// GUI

// EDITOR
#include "../EditorInteraction/Command.h"
#include "../EditorInteraction/DataManager.h"
// EDITOR

#define SHARED_MEMORY_NAME_CONTEX "Engine2 Editor Contex"
#define SHARED_MEMORY_NAME_DATA "Engine2 Editor Data"
constexpr int SHARED_MEMORY_SIZE_CONTEX = 1280 * 720 * 4;
constexpr int SHARED_MEMORY_SIZE_DATA = 1280 * 720 * 4;
Primative::Model GameScene::quadModel = { };
std::vector<GameEventsTypes> GameScene::getCurrentEvents() const
{
	std::vector<GameEventsTypes> res = {
		GameEventsTypes::Update
	};
	if (isFirstLoop) 
		res.insert(res.begin(), GameEventsTypes::Start);
	if (Events::Handler::buttonDown)
		res.push_back(GameEventsTypes::MouseToggle);
	if (Events::Handler::mouseMove)
		res.push_back(GameEventsTypes::MouseMove);
	if (Events::Handler::keyDown)
		res.push_back(GameEventsTypes::KeyToggle);
	return res;
}

void GameScene::processCommands()
{
	char* data = reinterpret_cast<char*>(sharedMemoryData.getData());
	if (data[0] == 2) { // is command
		EditorInteraction::Command command = EditorInteraction::Command(&data[1]);
		EditorInteraction::DataManager::executeCommand(command);
		EditorInteraction::CommandType t = command.getCommandType();
		Utils::log(std::to_string(static_cast<byte>(command.getCommandID())));
		int f = 0;
	}
	else { // is response or invalid

	}
}

GameScene::GameScene(bool renderToSharedMemory) : objects(), preProcessingLayers(), currentTick(0), postProcShaderId(0), FBOs_pre(), backgroundColour(0), 
							sharedMemoryContex(SHARED_MEMORY_NAME_CONTEX, SHARED_MEMORY_SIZE_CONTEX), sharedMemoryData(SHARED_MEMORY_NAME_DATA, SHARED_MEMORY_SIZE_DATA),
							skybox(nullptr), activeContext(nullptr), opaque(), transparent(), mainCamera(nullptr), terrain(), mainShadowCaster(nullptr), renderToSharedMemory(renderToSharedMemory),
							isFirstLoop(false), closing(false), uiStuff(), screenDimentions(0), emmiters(), lightSources(), USE_SHADOWS(1), uiContainers()
{
	quadModel = ResourceLoader::getModel("plane.dae");
	EditorInteraction::DataManager::setScene(this);
	if (renderToSharedMemory) {
		sharedMemoryContex.openFile();
		sharedMemoryData.openFile();
		processCommands();
	}
}

void GameScene::drawOpaque()
{
	for (Component::RenderMesh* mesh : opaque) {
		if(mesh->getParent()->isAlive())
			mesh->render(activeContext->getTime().deltaTime);
	}
}

void GameScene::drawTransparent(bool bindShader)
{
	if(bindShader)
		Render::Shading::Manager::setActive(ResourceLoader::getShader("TransparentShader")); 
	bindLights();
	assert(mainCamera);
	if (NOT transparent.size()) {
		return;
	}
	activeContext->enable(GL_BLEND);

	std::map<float, Component::RenderMesh*> sorted{};
	for (auto itt = transparent.rbegin(); itt != transparent.rend(); itt++) {
		float dist = (*itt).first;
		Component::RenderMesh* mesh = (*itt).second;
		if (mesh->getParent()->isAlive())
			mesh->render(activeContext->getTime().deltaTime);
		dist = glm::length2(mesh->getParent()->getGlobalTransform().Position - mainCamera->getPosition());
		sorted[dist] = mesh;
	}
	transparent.clear();
	transparent = sorted;
}

void GameScene::drawTerrain(bool bindShader)
{
	for (Terrain* terrain : this->terrain) {
		terrain->render(activeContext->getTime().deltaTime, bindShader);
	}
}

void GameScene::drawUI()
{
	for (UI::Canvas* canvas : uiStuff) {
		canvas->render(activeContext->getTime().deltaTime);
	}
	activeContext->disable(GL_CULL_FACE);
	activeContext->disable(GL_DEPTH);
	for (auto container : uiContainers) {
		container->render();
	}
	activeContext->enable(GL_CULL_FACE);
	activeContext->enable(GL_DEPTH);
}

void GameScene::drawParticles(Unsigned shaderId)
{
	unsigned t = Component::ParticleEmmiter::getShader();
	if (shaderId > 0)
		Component::ParticleEmmiter::setShader(shaderId);
	for (Component::ParticleEmmiter* part : emmiters) {
		part->render(activeContext->getTime().deltaTime);
	}
	Component::ParticleEmmiter::setShader(t);
}

void GameScene::addObject(GameObject* obj)
{
	objects.push_back(obj);
	obj->raiseEvents({ GameEventsTypes::Awake }, 0);
	obj->setScene(this);

	obj->processComponets();
}

void GameScene::addTerrain(Terrain* terrain)
{
	this->terrain.push_back(terrain);
}

void GameScene::addUI(GUI::GUIContainerBase* container)
{
	uiContainers.push_back(container);
}

void GameScene::addContext(Context* context)
{
	contexts.push_back(context);
}

void GameScene::processComponet(Component::ComponetBase* comp)
{

	Component::RenderMesh* mesh = dynamic_cast<Component::RenderMesh*>(comp);
	if (mesh) {
		if (mesh->getTransparent()) {
			transparent[rand()] = mesh;
		}
		else {
			opaque.push_back(mesh);
		}
		return;
	}
	UI::Canvas* canv = dynamic_cast<UI::Canvas*>(comp);
	if (canv) {
		uiStuff.push_back(canv);
		return;
	}
	Component::ParticleEmmiter* part = dynamic_cast<Component::ParticleEmmiter*>(comp);
	if (part) {
		emmiters.push_back(part);
		return;
	}
	Component::LightBase* light = dynamic_cast<Component::LightBase*>(comp);
	if (light) {
		lightSources.push_back(light);
	}
}

void GameScene::createStartUpFile(String location)
{
	std::string data = "";

	data += "Screen Dimentions:";
	data += Utils::to_string(*screenDimentions) + "\n";

	data += "Share Memory Data:";
	data += SHARED_MEMORY_NAME_CONTEX + std::string(":") + std::to_string(SHARED_MEMORY_SIZE_CONTEX) + "\n";

	Utils::writeToFile(location + "\\StartUp File.txt", data);
}

void GameScene::reSize(SVector2 newSize)
{
	activeContext->setDimentions(newSize);

	// re calculate projection
	glm::mat4 projection = glm::perspective(glm::radians(mainCamera->getFOV()), activeContext->getAspectRatio(), 0.01f, 5000.0f);
	uniformBuffers[0].fill(1, glm::value_ptr(projection));

	// reSize frame buffers
	for (auto& frameBuffer : FBOs_pre) {
		frameBuffer.second.reSize(newSize);
	}
	for (auto& item : FBOs_post) {
		for (auto& frameBuffer : item.second) {
			frameBuffer.reSize(newSize);
		}
	}
	
	// update UI projection
	UI::UIRenderer::updateProjection(newSize);
}

void GameScene::reSize(SVector2 aspectRatio, Short width)
{
	short height = static_cast<short>((static_cast<float>(aspectRatio.y) / static_cast<float>(aspectRatio.x)) * static_cast<float>(width));
	reSize({ width, height });
}

void GameScene::setBG(Vector3 col) 
{ 
	backgroundColour = col; 
};

void GameScene::preProcess()
{
	glm::mat4 lsMatrix(1);
	if(mainShadowCaster)
		lsMatrix = mainShadowCaster->getLSMatrix();
	for (const auto& layer : preProcessingLayers) {
		String name = layer.first;
		Unsigned shaderId = layer.second;
		Primative::Buffers::FrameBuffer& fbo = getFBO(name);
		fbo.bind();
		fbo.clearBits();
		Render::Shading::Manager::setActive(shaderId);

		if (name == "DirectionalShadows" AND mainShadowCaster) {

			Render::Shading::Manager::setValue("LSMatrix", lsMatrix);
			glCullFace(GL_FRONT);
			drawOpaque();

			drawTerrain(false);

			drawTransparent(false);

			// drawParticles(shaderId);
			glCullFace(GL_BACK);
		}
		else if(name == "G-Buffer")
		{
			drawOpaque();
			drawTerrain();
		}
		else if (name == "LightingPass") {
			int unit = 0;
			bindLights();
			Primative::Buffers::FrameBuffer& gBuffer = FBOs_pre["G-Buffer"];
			gBuffer.activateColourTextures(unit, { "positionTex", "albedoTex", "normalTex", "emissionTex", "MetRouAOTex" });

			Primative::Buffers::FrameBuffer& shadowBuffer = FBOs_pre["DirectionalShadows"];

			glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(GL_TEXTURE_2D, shadowBuffer.getTexture("depth"));
			bool suc = Render::Shading::Manager::setValue("ShadowMap", unit);

			if (mainShadowCaster) {
				Render::Shading::Manager::setValue("LSMatrix", lsMatrix);
				Render::Shading::Manager::setValue("ShadowCasterPosition", mainShadowCaster->getPosition());
			}
			// the deferred quad
			const Primative::Buffers::VertexBuffer& buffer = ResourceLoader::getBuffer(quadModel.getBuffers()[0]);
			buffer.render();

			// copy depth buffer for fwd rendering
			gBuffer.bind(GL_READ_FRAMEBUFFER);
			fbo.bind(GL_DRAW_FRAMEBUFFER);
			glBlitFramebuffer(0, 0, gBuffer.getDimentions().x, gBuffer.getDimentions().y, 0, 0, fbo.getDimentions().x, fbo.getDimentions().y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			fbo.unBind();
			gBuffer.unBind(GL_READ_FRAMEBUFFER);
			fbo.unBind(GL_DRAW_FRAMEBUFFER);
			fbo.bind();


			drawSkyBox();
			Render::Shading::Manager::setActive(ResourceLoader::getShader("TransparentShader"));
			Render::Shading::Manager::setValue("LSMatrix", lsMatrix);
			if(mainShadowCaster)
				Render::Shading::Manager::setValue("ShadowCasterPosition", mainShadowCaster->getPosition());
			drawTransparent(false);
			drawParticles();
			drawUI();
		}
		fbo.unBind();
	}
}

void GameScene::postProcess()
{
	if(renderToSharedMemory) {
		glUseProgram(0);
		glReadPixels(0, 0, screenDimentions->x, screenDimentions->y, GL_BGRA, GL_UNSIGNED_BYTE, sharedMemoryContex.getData());
		glUseProgram(0);
	}

	const Primative::Buffers::VertexBuffer& buffer = ResourceLoader::getBuffer(quadModel.getBuffers()[0]);
	for (auto& layer : postProcessingLayers) {
		String layerName = layer.first;
		Unsigned shaderId = layer.second;
		auto& fbos = getFBOs(layerName);
		Render::Shading::Manager::setActive(shaderId);
		if (layerName == "Blur" AND USES_BLUR) {
			bool first_itteration = true;
			bool horizontal = true;
			for (int i = 0; i < BLUR_ITTERATIONS; i++) {
				auto& fbo = fbos[horizontal];
				fbo.bind();
				fbo.clearBits();
				Render::Shading::Manager::setValue("horizontal", horizontal);
				horizontal = NOT horizontal;
				int unit = 0;
				if (first_itteration) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, FBOs_pre["LightingPass"].getTexture("col1"));
				}
				else {
					fbos[horizontal].activateColourTextures(unit, { "tex" });
				}
				buffer.render();
				first_itteration = false;
			}
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // default fbo
	clearFBO();
	
	Render::Shading::Manager::setActive(postProcShaderId);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOs_pre["LightingPass"].getTexture("col0"));
	if (USES_BLUR) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, FBOs_post["Blur"][0].getTexture("col0"));
		Render::Shading::Manager::setValue("bloomBlur", 1);
	}

	Render::Shading::Manager::setValue("flip", renderToSharedMemory ? 1 : -1);
	Render::Shading::Manager::setValue("scene", 0);
	Render::Shading::Manager::setValue("Blur", USES_BLUR);
	
	buffer.render();
}

void GameScene::updateObjects()
{
	const auto events = getCurrentEvents();
	for (GameObject*& obj : objects) {
		if (obj->isAlive()) {
			//obj->tick(currentTick++ % FIXED_UPDATE_RATE, mainContext.getTime().deltaTime);
			obj->raiseEvents(events, activeContext->getTime().deltaTime);
		}
	}

	for (auto container : uiContainers) {
		container->handleEvents();
	}
}

void GameScene::clearFBO() const 
{
	glViewport(0, 0, screenDimentions->x, screenDimentions->y);
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
	activeContext->disable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	skybox->draw();
	glDepthFunc(GL_LESS);
	activeContext->enable(GL_CULL_FACE);
}

Primative::Buffers::FrameBuffer& GameScene::getFBO(String name)
{
	if (name == "any") {
		return (*FBOs_pre.begin()).second;
	}
	Unsigned s = FBOs_pre.size();
	auto& r = FBOs_pre[name];
	if (s < FBOs_pre.size()) {
		FBOs_pre.erase(name);
		return (*(FBOs_pre.begin()++)).second;
	}
	return r;
}

std::vector<Primative::Buffers::FrameBuffer>& GameScene::getFBOs(String name)
{
	const int s = FBOs_post.size();
	auto& r = FBOs_post[name];
	if (s != FBOs_post.size()) {
		FBOs_post.erase(name);
		return (*(FBOs_post.begin()++)).second;
	}
	return r;
}

Component::Camera* GameScene::getMainCamera()
{
	return mainCamera;
}

const Component::ShadowCaster* GameScene::getShadowCaster() const
{
	return mainShadowCaster;
}

Context* GameScene::getActiveContext() const
{
	return activeContext;
}

SharedMemoryLocation& GameScene::getSharedData()
{
	return sharedMemoryData;
}

void GameScene::initalize()
{
#pragma region Pre Processing
	Primative::Buffers::FrameBuffer shadow_FBO({ "depth" }, *screenDimentions, GL_CLAMP_TO_BORDER);
	Primative::Buffers::FrameBuffer g_buffer_FBO({ "col0", "col1", "col2", "col3", "col4" }, *screenDimentions);
	Primative::Buffers::FrameBuffer lighting_FBO({ "col0", "col1" }, *screenDimentions);


	addFBO_Pre("DirectionalShadows", shadow_FBO);
	addPreProcLayer("DirectionalShadows", ResourceLoader::getShader("ShadowShader"));

	addFBO_Pre("G-Buffer", g_buffer_FBO);
	addPreProcLayer("G-Buffer", ResourceLoader::getShader("DeferredOpaque"));

	addFBO_Pre("LightingPass", lighting_FBO);
	addPreProcLayer("LightingPass", ResourceLoader::getShader("DeferredFinal"));

	/*if(NOT USE_DEFFERED OR true) {
		Primative::Buffers::FrameBuffer shadowFBO({ "depth" }, screenDimentions, { 1, 0, 1 });
		Primative::Buffers::FrameBuffer finalFBO({ "col0" }, screenDimentions, { 0, 0, 0 });
		addFBO_Pre("shadows", shadowFBO);
		addPreProcLayer("shadows", ResourceLoader::getShader("ShadowShader"));

		addFBO_Pre("final", finalFBO);
		addPreProcLayer("final", ResourceLoader::getShader("PBRShader"));
	}*/

	setPostProcShader(ResourceLoader::getShader("PostShader")); // PBRShader  PostShader
#pragma endregion

#pragma region Post Processing
	if (USES_BLUR) {
		Primative::Buffers::FrameBuffer blur1_FBO({ "col0" }, *screenDimentions);
		Primative::Buffers::FrameBuffer blur2_FBO({ "col0" }, *screenDimentions);
		addFBO_Post("Blur", blur1_FBO);
		addFBO_Post("Blur", blur2_FBO);
		addPostProcLayer("Blur", ResourceLoader::getShader("GausianBlur"));
	}
#pragma endregion

	
	Primative::Buffers::StaticBuffer mainBuffer("m4, m4, v3, f, f", 0);
	// view    | matrix 4
	// proj    | matrix 4
	// viewPos | vector 3
	// gamma   | scalar f

	glm::mat4 projection = glm::perspective(glm::radians(mainCamera->getFOV()), static_cast<float>(screenDimentions->x) / static_cast<float>(screenDimentions->y), 0.01f, 5000.0f);
	mainBuffer.fill(1, glm::value_ptr(projection));

	float gamma = 2.2f;
	mainBuffer.fill(3, &gamma);
	if(mainCamera)
		mainBuffer.fill(4, &mainCamera->getExposure());

	uniformBuffers.push_back(mainBuffer);

	createStartUpFile("C:\\Users\\AGWDW\\Desktop");
}
//Context mainC;
void GameScene::gameLoop()
{
	// float counter = 0;
	isFirstLoop = true;
	while (NOT (closing OR activeContext->shouldClose()))
	{
		processCommands();
		if (Events::Handler::getKey(Events::Key::Backspace, Events::Action::Down)) {
			/*mainC = Context({ 400, 400 }, false);
			mainC.init("Engine 2", { GL_BLEND, GL_DEPTH_TEST, GL_CULL_FACE, GL_MULTISAMPLE, GL_SCISSOR_TEST });
			addContext(&mainC);
			setActiveContext(1);*/
			reSize({ 1, 1 }, 400);
		}


		// FPS--------------------------------------------------------------------------------------------------------------------------------------------------

		// tb.setText("FPS: " + std::to_string(main.getTime().avgFPS));
		// const float w = tb.getWidth() / 2.0f;
		// const glm::vec2& p = tb.getPos();
		// float diff = p.x - w; // left edge
		// diff = 5 - diff;
		// tb.setPos({ p.x + diff, p.y });

		// UPDATES----------------------------------------------------------------------------------------------------------------------------------------------
		updateObjects();

		// counter += main.getTime().deltaTime;

		// RENDERING--------------------------------------------------------------------------------------------------------------------------------------------
		auto& mainBuffer = uniformBuffers[0];
		mainBuffer.fill(0, glm::value_ptr(mainCamera->getView()));
		mainBuffer.fill(2, glm::value_ptr(mainCamera->getPosition()));

		preProcess(); // shadows and scene quad
		postProcess();// render to screen
		Gizmos::GizmoRenderer::drawAll(renderToSharedMemory);

		// sound->update();

		activeContext->update();
		Events::Handler::pollEvents();
		isFirstLoop = false;
	}
}

void GameScene::cleanUp()
{
	for (auto& obj : objects) {
		obj->cleanUp();
		//delete obj;
		obj = nullptr;
	}
	objects.clear();
	for (auto& pair : FBOs_pre) {
		Primative::Buffers::FrameBuffer& fbo = pair.second;
		fbo.cleanUp();
	}
	for (auto& pair : FBOs_post) {
		for (auto itt = pair.second.begin(); itt != pair.second.end();) {
			(*itt).cleanUp();
			itt = pair.second.erase(itt);
		}
	}
	for (auto itt = uniformBuffers.begin(); itt != uniformBuffers.end();) {
		(*itt).cleanUp();
		itt = uniformBuffers.erase(itt);
	}
	for (auto itt = terrain.begin(); itt != terrain.end();) {
		(*itt)->cleanUp();
		itt = terrain.erase(itt);
	}
	for (auto itt = uiStuff.begin(); itt != uiStuff.end();) {
		(*itt)->cleanUp();
		itt = uiStuff.erase(itt);
	}
	for (auto itt = emmiters.begin(); itt != emmiters.end();) {
		(*itt)->cleanUp();
		itt = emmiters.erase(itt);
	}
	for (auto itt = lightSources.begin(); itt != lightSources.end();) {
		(*itt)->cleanUp();
		itt = lightSources.erase(itt);
	}
	for (auto itt = uiContainers.begin(); itt != uiContainers.end();) {
		(*itt)->cleanUp();
		itt = uiContainers.erase(itt);
	}

	//delete[] ColourBuffer;
	// ColourBuffer = nullptr;
	//sharedMemory.closeFile();

	FBOs_pre.clear();
	FBOs_post.clear();
	if(skybox)
		skybox->cleanUp();
	skybox = nullptr;
	mainCamera = nullptr;
	opaque.clear();
	transparent.clear();
	quadModel.cleanUp();
	for (auto itt = contexts.begin(); itt != contexts.end();) {
		(*itt)->cleanUp();
		itt = contexts.erase(itt);
	}
	if (renderToSharedMemory) {
		sharedMemoryContex.closeFile();
		sharedMemoryData.closeFile();
	}
	EditorInteraction::DataManager::cleanUp();
}

void GameScene::close()
{
	closing = true;
}

void GameScene::bindLights()
{
	int directional = 0;
	int point = 0;
	int spot = 0;
	for (Component::LightBase* light : lightSources) {
		int count = -1;
		std::string type = "";
		switch (light->getLightType())
		{
		case Component::LightType::Directional:
			count = directional++;
			type = "directional";
			break;
		case Component::LightType::Point:
			count = point++;
			type = "point";
			break;
		case Component::LightType::Spot:
			count = spot++;
			type = "spot";
			break;
		}
		Render::Shading::Manager::setValue(type, light, count);
	}
	Render::Shading::Manager::setValue("numberOfDirectionalLights", directional);
	Render::Shading::Manager::setValue("numberOfPointLights", point);
	Render::Shading::Manager::setValue("numberOfSpotLights", spot);
}

void GameScene::addPreProcLayer(String name, Unsigned shaderId)
{
	//this->preProcessingLayers[name] = shaderId;	
	preProcessingLayers.push_back({ name, shaderId });
}

void GameScene::addPostProcLayer(String name, Unsigned shaderId)
{
	postProcessingLayers.push_back({ name, shaderId });
}

void GameScene::addFBO_Pre(String layerName, Primative::Buffers::FrameBuffer fbo)
{
	FBOs_pre[layerName] = fbo;
}
void GameScene::addFBO_Post(String layerName, Primative::Buffers::FrameBuffer fbo)
{
	FBOs_post[layerName].push_back(fbo);
}

void GameScene::setPostProcShader(Unsigned shaderId)
{
	postProcShaderId = shaderId;
};

void GameScene::setSkyBox(SkyBox* sb)
{
	skybox = sb;
}

void GameScene::setMainCamera(Component::Camera* camera)
{
	mainCamera = camera;
	if(mainShadowCaster)
		mainShadowCaster->setCamera(mainCamera);
	if(uniformBuffers.size())
		uniformBuffers.front().fill(4, &mainCamera->getExposure());
}

void GameScene::setShadowCaster(Component::ShadowCaster* caster)
{
	mainShadowCaster = caster;
	mainShadowCaster->setCamera(mainCamera);
}

void GameScene::setActiveContext(Unsigned index)
{
	activeContext = contexts[index];
	glfwMakeContextCurrent(nullptr);
	glfwMakeContextCurrent(activeContext->getWindow());
	screenDimentions = activeContext->getDimentions();
}

glm::svec2* GameScene::getScreenDimentions()
{
	return screenDimentions;
}

GameObject* GameScene::getObject(String name)
{
	for (GameObject* obj : objects) {
		if (obj->getName() == name) {
			return obj;
		}
	}
	return nullptr;
}
