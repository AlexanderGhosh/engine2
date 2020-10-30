#pragma once
#include <vector>
#include <unordered_map>
#include "GameObject.h"
#include "../Primatives/Buffers.h"
class GameScene
{
private:
	std::vector<GameObject*> objects;
	std::unordered_map<std::string, unsigned> preProcessingLayers;
	unsigned currentTick, postProcShaderId;
	std::unordered_map<std::string, Primative::FrameBuffer*> FBOs;
	inline void clearFBO() const {
		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
public:
	inline GameScene() : objects(), preProcessingLayers(), currentTick(0), postProcShaderId(0), FBOs() { };
	inline void addObject(GameObject*& obj) { objects.push_back(obj); obj = nullptr; };
	void preProcess();
	void postProcess();
	void renderObjects();
	const Primative::FrameBuffer* getFBO(const std::string& name);
	void cleanUp();
	void addPreProcLayer(const std::string& name, const unsigned& shaderId);
	inline void addFBO(const std::string& layerName, Primative::FrameBuffer*& fbo) { FBOs.insert({ layerName, fbo }); fbo = nullptr; };
	inline void setPostProcShader(const unsigned& shaderId) { postProcShaderId = shaderId; };
};

