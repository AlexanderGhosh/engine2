#pragma once
#include "../Utils/General.h"
#include "../Componets/Scripting.h"
class PlayerControler : public Component::Scripting
{
public:
	PlayerControler();
	~PlayerControler() = default;
	void update(float deltaTime);
	void mouseMove(float deltaTime);
private:
	bool firstMouse;
	float lastX, lastY;
};

