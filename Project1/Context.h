#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm.hpp>
#include <list>
#include <string>
#include "Utils/General.h"

class Time {
public:
	float fps;
	short avgFPS;
	float deltaTime;
	float timeElapsed;
	float prevFrameTime;
	short sampleRate;
	void update();
	Time();
private:
	float fpsCount;
	short frameCounter;
};
class Context
{
private:
	GLFWwindow* window;
	glm::svec2 dimentions;
	bool vsync;
	short fpsLim;
	Time time;
	std::string name;
	std::list<long> enabled;
public:
	Context();
	Context(const glm::svec2& dim, const bool vsync, short fpsLim = 0);
	Context(const glm::svec2& aspectRatio, short horizontalLength, const bool vsync, short fpsLim);

	void init(const std::string& name, std::list<long> enable, bool initGlew = true);
	void enable(long a);
	void disable(long a);
	void update();
	bool shouldClose() const;

	void cleanUp(bool terminate = true);
	void remove();

	// getters
	unsigned    getWidth()      const;
	unsigned    getHeight()     const;
	glm::svec2*	    getDimentions();
	GLFWwindow*& getWindow();
	Time&		getTime();
	float getAspectRatio() const;

	// setters
	void setDimentions(SVector2 a);
};

