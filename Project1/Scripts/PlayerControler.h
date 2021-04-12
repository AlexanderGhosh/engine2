#pragma once
#include "../Utils/General.h"
#include "../Componets/Scripting.h"
class Terrain;
class PlayerControler : public Component::Scripting
{
public:
	PlayerControler();
	~PlayerControler() = default;
	void update(float deltaTime);
	void mouseMove(float deltaTime);
	Terrain* ground;
private:
	/// <summary>
	/// will move player to the ground hight if y < h
	/// </summary>
	/// <returns></returns>
	bool isGrounded();
	bool firstMouse;
	float lastX, lastY;
};

