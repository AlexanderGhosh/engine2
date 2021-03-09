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
namespace Component {
	class Camera;
	class RenderMesh;
}
namespace Primative {
	class Model;
	namespace Buffers {
		class FrameBuffer;
	}
}
class GameScene
{
private:
	Primative::Model quadModel;
	std::vector<Terrain*> terrain;
	SkyBox* skybox;
	Component::Camera* mainCamera;
	std::vector<Component::RenderMesh*> opaque;
	std::map<float, Component::RenderMesh*> transparent;
	std::vector<GameObject*> objects;
	std::unordered_map<std::string, unsigned> preProcessingLayers;
	unsigned currentTick, postProcShaderId;
	std::unordered_map<std::string, Primative::Buffers::FrameBuffer*> FBOs;
	glm::vec3 backgroundColour;
	Context* mainContext;
	void clearFBO() const;
	void drawObjects(Unsigned shaderId); // just moved
	void drawSkyBox();
	void drawOpaque();
	void drawTransparent();
	void drawTerrain();
public:
	GameScene();
	void preProcess();
	void postProcess();
	void updateScene();
	const Primative::Buffers::FrameBuffer* getFBO(const std::string& name);
	void cleanUp();

	// adders
	void addPreProcLayer(const std::string& name, const unsigned& shaderId);
	void addFBO(const std::string& layerName, Primative::Buffers::FrameBuffer* fbo);
	void addObject(GameObject* obj);
	void addTerrain(Terrain* terrain);

	// setters
	void setBG(Vector3 col);
	void setPostProcShader(const unsigned& shaderId);
	void setSkyBox(SkyBox* sb);
	void setContext(Context* context);
	void setMainCamera(Component::Camera* camera);
};

