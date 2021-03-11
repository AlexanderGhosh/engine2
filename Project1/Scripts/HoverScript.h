#pragma once
#include "../Utils/General.h"
#include "../Componets/Scripting.h"
class HoverScript : public Component::Scripting
{
private:
	float distance, speed, moved;
public:
	HoverScript();
	HoverScript(Float dist, Float speed);
	~HoverScript() = default;
	void update(float deltaTime);
};

