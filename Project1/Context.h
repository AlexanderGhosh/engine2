#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm.hpp>
#include <list>
#include <string>

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
public:
	using svec2 = glm::vec<2, short, glm::packed_highp>;
private:
	GLFWwindow* window;
	svec2 dimentions;
	bool vsync;
	short fpsLim;
	Time time;
public:
	Context(const svec2& dim, const bool vsync, short fpsLim = -1);

	void init(const std::string& name, std::list<long> enable);
	void enable(long a) const;
	void disable(long a) const;
	void update();
	bool shouldClose() const;

	void cleanUp();
	void remove();

	// getters
	unsigned    getWidth()      const;
	unsigned    getHeight()     const;
	svec2	    getDimentions() const;
	GLFWwindow* getWindow()     const;

	Time&		getTime();

	// setters
};

