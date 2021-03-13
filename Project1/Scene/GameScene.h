#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "../Primatives/Model.h"
#include "../Utils/General.h"

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

	std::vector<GameObject*> objects;
	std::unordered_map<std::string, unsigned> preProcessingLayers;
	unsigned currentTick, postProcShaderId;
	std::unordered_map<std::string, Primative::Buffers::FrameBuffer> FBOs;
	glm::vec3 backgroundColour;
	Context* mainContext;
	std::vector<Primative::Buffers::StaticBuffer> uniformBuffers;
	bool isFirstLoop, closing;
	glm::ivec2 screenDimentions;

	void clearFBO() const;
	void drawObjects(Unsigned shaderId); // just moved
	void drawSkyBox();
	void drawOpaque();
	void drawTransparent();
	void drawTerrain();
	void drawUI();
	void drawParticles();

	// polling evetns
	std::vector<GameEventsTypes> getCurrentEvents() const;
public:
	GameScene();
	void preProcess();
	void postProcess();
	void updateObjects();
	const Primative::Buffers::FrameBuffer& getFBO(const std::string& name);
	void initalize();
	void gameLoop();
	void cleanUp();
	void close();

	// adders
	void addPreProcLayer(const std::string& name, const unsigned& shaderId);
	void addFBO(const std::string& layerName, Primative::Buffers::FrameBuffer fbo);
	void addObject(GameObject* obj);
	void addTerrain(Terrain* terrain);
	void processComponet(Component::ComponetBase* comp);

	// setters
	void setBG(Vector3 col);
	void setPostProcShader(const unsigned& shaderId);
	void setSkyBox(SkyBox* sb);
	void setContext(Context* context);
	void setMainCamera(Component::Camera* camera);

	// getters
	const glm::ivec2& getScreenDimentions() const;
	/// <summary>
	/// returns the first gameobject with the given name
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	GameObject* getObject(String name);
};

