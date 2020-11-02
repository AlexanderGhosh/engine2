#include "Handler.h"

GLFWwindow* Events::Handler::window = nullptr;

void Events::Handler::init(GLFWwindow* window)
{
	Events::Handler::window = window;
	glfwSetKeyCallback(window, Events::Handler::keyCallBack);
	glfwSetCursorPosCallback(window, Events::Handler::mouse_callback);
	// glfwSetKeyCallback(window, NULL);
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
	glm::dvec2 res;
	glfwGetCursorPos(Events::Handler::window, &res.x, &res.y);
	return static_cast<glm::vec2>(res);
}

void Events::Handler::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{

}

void Events::Handler::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}
