#pragma once
#include "../Componets/Scripting.h"

namespace Component {
	class ParticleEmmiter;
}

class RainMakerScript : public Component::Scripting
{
private:
	Component::ParticleEmmiter* emmiter;
public:
	RainMakerScript();
	~RainMakerScript() = default;
	void cleanUp();
	void start();
	void update(float deltaTime);
};

