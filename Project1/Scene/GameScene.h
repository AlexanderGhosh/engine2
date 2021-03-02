#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "../GameObject/GameObject.h"
#include "../Primatives/Buffers.h"
class SkyBox;
class Context;
namespace Component {
	class RenderMesh;
	class Camera;
}
class GameScene
{
private:
	Primative::FrameBuffer* quadFBO;
	Primative::VertexBuffer quadBuffer;
	SkyBox* skybox;
	Component::Camera* mainCamera;
	std::vector<GameObject*> objects;
	std::unordered_map<std::string, unsigned> preProcessingLayers;
	unsigned currentTick, postProcShaderId;
	std::unordered_map<std::string, Primative::FrameBuffer*> FBOs;
	glm::vec3 backgroundColour;
	Context* mainContext;
	std::vector<Component::RenderMesh*> opaque;
	std::map<float, Component::RenderMesh*> transparent;
	inline void clearFBO() const {
		glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void renderObjects(); // just moved
	void renderSkyBox();
	void drawOpaque();
	/// <summary>
	/// draws all transparent objects and sorts them for the next frame
	/// </summary>
	void drawTransparent();
public:
	GameScene();
	void addObject(GameObject* obj);
	inline void setBG(Vector3 col) { backgroundColour = col; };
	void preProcess();
	void postProcess();
	void updateScene();
	const Primative::FrameBuffer* getFBO(const std::string& name);
	void cleanUp();
	void addPreProcLayer(const std::string& name, const unsigned& shaderId);
	inline void addFBO(const std::string& layerName, Primative::FrameBuffer* fbo) 
	{ 
		FBOs.insert({ layerName, fbo }); 
		fbo = nullptr; 
	};

	// setters
	inline void setPostProcShader(const unsigned& shaderId) { postProcShaderId = shaderId; };
	void setSkyBox(SkyBox* sb);
	void setContext(Context* context);
	void setMainCamera(Component::Camera* camera);
};

