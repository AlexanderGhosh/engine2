#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "../Primatives/Model.h"
#include "../Utils/General.h"

#define BLUR_ITTERATIONS 10
#define USES_BLUR false

class SkyBox;
class Context;
class GameObject;
class Terrain;
namespace UI {
	class Canvas;
}
namespace Component {
	class ComponetBase;
	class Camera;
	class RenderMesh;
	class ParticleEmmiter;
	class LightBase;
	class ShadowCaster;
}
namespace Primative {
	class Model;
	namespace Buffers {
		class FrameBuffer;
		class StaticBuffer;
	}
}
enum class GameEventsTypes {
	Update, FixedUpdate, MouseToggle, MouseMove, KeyToggle,
	Render,
	Awake, // will occour when object is added to the scene
	Start  // will occout before first game loop
};
class GameScene
{
private:
	Primative::Model quadModel;
	std::vector<Terrain*> terrain;
	SkyBox* skybox;
	Component::Camera* mainCamera;

	std::vector<Component::RenderMesh*> opaque;
	std::map<float, Component::RenderMesh*> transparent;
	std::vector<UI::Canvas*> uiStuff;
	std::vector<Component::ParticleEmmiter*> emmiters;
	std::vector<Component::LightBase*> lightSources;
	Component::ShadowCaster* mainShadowCaster;

	std::vector<GameObject*> objects;
	std::unordered_map<std::string, unsigned> preProcessingLayers;
	std::unordered_map<std::string, unsigned> postProcessingLayers;
	unsigned currentTick, postProcShaderId;
	std::unordered_map<std::string, Primative::Buffers::FrameBuffer> FBOs_pre;
	std::unordered_map<std::string, std::vector<Primative::Buffers::FrameBuffer>> FBOs_post;
	glm::vec3 backgroundColour;
	Context* mainContext;
	std::vector<Primative::Buffers::StaticBuffer> uniformBuffers;
	bool isFirstLoop, closing;
	glm::ivec2 screenDimentions;


	void clearFBO() const;
	void drawObjects(Unsigned shaderId); // just moved
	void drawSkyBox();
	void drawOpaque();
	void drawTransparent(bool bindShader = true);
	void drawTerrain(bool bindShader = true);
	void drawUI();
	void drawParticles();

	// polling evetns
	std::vector<GameEventsTypes> getCurrentEvents() const;
public:	
	bool USE_SHADOWS;
	GameScene();
	void preProcess();
	void postProcess();
	void updateObjects();
	void initalize();
	void gameLoop();
	void cleanUp();
	void close();
	/// <summary>
	/// binds the lights to the active shader
	/// </summary>
	void bindLights();

	// adders
	void addPreProcLayer(String name, Unsigned shaderId);
	void addPostProcLayer(String name, Unsigned shaderId);
	void addFBO_Pre(String layerName, Primative::Buffers::FrameBuffer fbo);
	void addFBO_Post(String layerName, Primative::Buffers::FrameBuffer fbo);
	void addObject(GameObject* obj);
	void addTerrain(Terrain* terrain);
	void processComponet(Component::ComponetBase* comp);

	// setters
	void setBG(Vector3 col);
	void setPostProcShader(Unsigned shaderId);
	void setSkyBox(SkyBox* sb);
	void setContext(Context* context);
	void setMainCamera(Component::Camera* camera);
	void setShadowCaster(Component::ShadowCaster* caster);

	// getters
	const glm::ivec2& getScreenDimentions() const;
	/// <summary>
	/// returns the first gameobject with the given name
	/// </summary>
	GameObject* getObject(String name);
	Primative::Buffers::FrameBuffer& getFBO(String name);
	std::vector<Primative::Buffers::FrameBuffer>& getFBOs(String name);
	Component::Camera* getMainCamera();
	const Component::ShadowCaster* getShadowCaster() const;
	Context* getContext() const;
};

