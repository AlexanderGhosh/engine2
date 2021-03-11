#pragma once
#include "../Utils/General.h"
#include "../Componets/Scripting.h"
class SpinScript : public Component::Scripting
{
private:
	glm::vec3 axis;
	float rps;
public:
	SpinScript();
	SpinScript(Vector3 axis, Float rps);
	~SpinScript() = default;
	void update(float deltaTime);
};

