#pragma once
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <string>
#include "../Utils/General.h"
namespace Events {
	enum class Action {
		Up = GLFW_RELEASE,
		Down = GLFW_PRESS,
		Repeat = GLFW_REPEAT
	};
	enum class Key {
		Zero = GLFW_KEY_0,
		One = GLFW_KEY_1, Two = GLFW_KEY_2, Three = GLFW_KEY_3, 
		Four = GLFW_KEY_4, Five = GLFW_KEY_5, Six = GLFW_KEY_6, 
		Seven = GLFW_KEY_7, Eight = GLFW_KEY_8, Nine = GLFW_KEY_9,
		Grave = GLFW_KEY_GRAVE_ACCENT, Tab = GLFW_KEY_TAB, CpsLock = GLFW_KEY_CAPS_LOCK, 
		Escape = GLFW_KEY_ESCAPE, UnderScore = GLFW_KEY_MINUS, Equal = GLFW_KEY_EQUAL, 
		Backspace = GLFW_KEY_BACKSPACE, Enter = GLFW_KEY_ENTER, SemiColon = GLFW_KEY_SEMICOLON,
		Apostrophe = GLFW_KEY_APOSTROPHE, Fdw_Slash = GLFW_KEY_SLASH, Bck_Slash = GLFW_KEY_BACKSLASH, 
		Comma = GLFW_KEY_COMMA, Period = GLFW_KEY_PERIOD,
		L_Shift = GLFW_KEY_LEFT_SHIFT, L_Control = GLFW_KEY_LEFT_CONTROL, L_Alt = GLFW_KEY_LEFT_ALT, 
		R_Shift = GLFW_KEY_RIGHT_SHIFT, R_Control = GLFW_KEY_RIGHT_CONTROL, R_Alt = GLFW_KEY_RIGHT_ALT,
		Up = GLFW_KEY_UP, Down = GLFW_KEY_DOWN, Left = GLFW_KEY_LEFT, Right = GLFW_KEY_RIGHT,
		Asterisk = GLFW_KEY_KP_MULTIPLY, Plus = GLFW_KEY_KP_ADD, Divide = GLFW_KEY_KP_DIVIDE,
		Minus = GLFW_KEY_KP_SUBTRACT, Num_Enter = GLFW_KEY_KP_ENTER,
		Num_0 = GLFW_KEY_KP_0,
		Num_1 = GLFW_KEY_KP_1, Num_2 = GLFW_KEY_KP_2, Num_3 = GLFW_KEY_KP_3, 
		Num_4 = GLFW_KEY_KP_4, Num_5 = GLFW_KEY_KP_5, Num_6 = GLFW_KEY_KP_6, 
		Num_7 = GLFW_KEY_KP_7, Num_8 = GLFW_KEY_KP_8, Num_9 = GLFW_KEY_KP_9,

		Space = GLFW_KEY_SPACE,
		A = GLFW_KEY_A, B = GLFW_KEY_B, C = GLFW_KEY_C,
		D = GLFW_KEY_D, E = GLFW_KEY_E, F = GLFW_KEY_F,
		G = GLFW_KEY_G, H = GLFW_KEY_H, I = GLFW_KEY_I,
		J = GLFW_KEY_J, K = GLFW_KEY_K, L = GLFW_KEY_L,
		M = GLFW_KEY_M, N = GLFW_KEY_N, O = GLFW_KEY_O,
		P = GLFW_KEY_P, Q = GLFW_KEY_Q, R = GLFW_KEY_R,
		S = GLFW_KEY_S, T = GLFW_KEY_T, U = GLFW_KEY_U,
		V = GLFW_KEY_V, W = GLFW_KEY_W, X = GLFW_KEY_X,
		Y = GLFW_KEY_Y, Z = GLFW_KEY_Z,
	};
	enum class Cursor {
		/*M1 = GLFW_MOUSE_BUTTON_1, 
		M2 = GLFW_MOUSE_BUTTON_2, 
		M3 = GLFW_MOUSE_BUTTON_3, 
		M4 = GLFW_MOUSE_BUTTON_4,*/
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Right = GLFW_MOUSE_BUTTON_RIGHT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE,
		Any = -1
	};
	class Handler
	{
	public:
		static void init(GLFWwindow* window);
		static inline void pollEvents() { glfwPollEvents(); };
		static bool getKey(const Key& key, const Action& action);
		static bool getCursor(const Cursor& button, const Action& state);
		static const glm::vec2& getCursorPos();
		static std::string lastChar;
		static bool keyDown, mouseMove, buttonDown;
		static glm::vec2 scrollOffest;
	private:
		static GLFWwindow* window;
		static void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode); 
		static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void characterCallBack(GLFWwindow* window, unsigned int codePoint);
		static void scrollInputCallBack(GLFWwindow* window, double xoffset, double yoffset);
	};
};

