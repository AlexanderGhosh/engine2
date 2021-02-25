#include "Context.h"
#include <iostream>

Context::Context(const svec2& dim, const bool vsync, short fpsLim) : window(nullptr), dimentions(dim), vsync(vsync), fpsLim(fpsLim), time()
{
}

void Context::init(const std::string& name, std::list<long> enable)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(dimentions.x, dimentions.y, name.c_str(), NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glEnable(GL_MULTISAMPLE);
    for (auto itt = enable.begin(); itt != enable.end(); itt++)
        glEnable(*itt);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, dimentions.x, dimentions.y);
    glewInit();
}

void Context::enable(long a) const
{
    glEnable(a);
}

void Context::disable(long a) const
{
    glDisable(a);
}

void Context::update()
{
    glfwSwapInterval(vsync ? 1 : fpsLim ); // V-SYNC
    glfwSwapBuffers(window);
    time.update();
}

bool Context::shouldClose() const
{
    return glfwWindowShouldClose(window);
}

void Context::cleanUp()
{
    glfwMakeContextCurrent(nullptr);
    glfwTerminate();
	window = nullptr;
}

void Context::remove()
{
	cleanUp();
}

unsigned Context::getWidth() const
{
	return dimentions.x;
}

unsigned Context::getHeight() const
{
	return dimentions.x;
}

Context::svec2 Context::getDimentions() const
{
	return dimentions;
}

GLFWwindow* Context::getWindow() const
{
    return window;
}

Time& Context::getTime()
{
    return time;
}

void Time::update()
{
    float currentTime = glfwGetTime();
    deltaTime = currentTime - timeElapsed;
    fps = 1.0f / deltaTime;
    fpsCount += fps;
    timeElapsed = currentTime;
    if (frameCounter >= sampleRate) {
        avgFPS = fpsCount / sampleRate;
        frameCounter = 0;
        fpsCount = 0;
        sampleRate = avgFPS * 0.5;
    }
    frameCounter++;
}

Time::Time() : fps(0), avgFPS(0), deltaTime(1.0f / 60.0f), timeElapsed(0), prevFrameTime(0), sampleRate(10), fpsCount(0), frameCounter(0)
{
}
