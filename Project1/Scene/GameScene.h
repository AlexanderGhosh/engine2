#pragma once
#include <vector>
#include <unordered_map>
#include "../GameObject/GameObject.h"
#include "../Primatives/Buffers.h"
class GameScene
{
private:
	std::vector<GameObject*> objects;
	std::unordered_map<std::string, unsigned> preProcessingLayers;
	unsigned currentTick, postProcShaderId;
	std::unordered_map<std::string, Primative::FrameBuffer*> FBOs;
	glm::vec3 backgroundColour;
	inline void clearFBO() const {
		glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void renderObjects(); // just moved
public:
	inline GameScene() : objects(), preProcessingLayers(), currentTick(0), postProcShaderId(0), FBOs(), backgroundColour(0) { };
	inline void addObject(GameObject*& obj) { objects.push_back(obj); };
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
	inline void setPostProcShader(const unsigned& shaderId) { postProcShaderId = shaderId; };
};

