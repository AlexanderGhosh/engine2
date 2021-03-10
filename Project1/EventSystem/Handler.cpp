#include "Handler.h"
#include "../Utils/General.h"
#include <iostream>


GLFWwindow* Events::Handler::window = nullptr;
std::string Events::Handler::lastChar = "";
bool Events::Handler::keyDown = false;
bool Events::Handler::mouseMove = false;
bool Events::Handler::buttonDown = false;

void Events::Handler::init(GLFWwindow* window)
{
	Events::Handler::window = window;
	glfwSetKeyCallback(window, Events::Handler::keyCallBack);
	glfwSetCursorPosCallback(window, Events::Handler::mouseCallback);
	glfwSetMouseButtonCallback(window, Events::Handler::mouseButtonCallback);
	glfwSetCharCallback(window, Events::Handler::characterCallBack);
	// glfwSetKeyCallback(window, NULL);
}

bool Events::Handler::getKey(const Key& key, const Action& action)
{
	return glfwGetKey(Events::Handler::window, static_cast<int>(key)) == static_cast<int>(action);
}

bool Events::Handler::getCursor(const Cursor& button, const Action& state)
{
	bool res = true;
	if (static_cast<int>(button) == -1) {
		for (short i = 0; i < 4 && res; i++) {
			res = res && Events::Handler::getCursor(static_cast<Events::Cursor>(GLFW_MOUSE_BUTTON_1 + i), state);
		}
		for (short i = 0; i < 3 && res; i++) {
			res = res && Events::Handler::getCursor(static_cast<Events::Cursor>(GLFW_MOUSE_BUTTON_LEFT + i), state);
		}
		return res;
	}
	return glfwGetMouseButton(Events::Handler::window, static_cast<int>(button)) == static_cast<int>(state);
}

const glm::vec2& Events::Handler::getCursorPos()
{
	glm::dvec2 res(0);
	glfwGetCursorPos(Events::Handler::window, &res.x, &res.y);
	return static_cast<glm::vec2>(res);
}

void Events::Handler::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	keyDown = action;
}

void Events::Handler::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouseMove = xpos != 0 OR ypos != 0;
}

void Events::Handler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	buttonDown = action;
}

void Events::Handler::characterCallBack(GLFWwindow* window, unsigned int codePoint)
{
	Events::Handler::lastChar = Utils::toUTF8(codePoint);
	// std::cout << Events::Handler::lastChar << std::endl;
}
