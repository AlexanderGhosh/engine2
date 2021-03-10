#pragma once
#include "../Utils/General.h"
#include "../Componets/Scripting.h"
class PlayerControler : public Component::Scripting
{
public:
	PlayerControler();
	void update(float deltaTime);
};

