#include "RainMakerScript.h"
#include "../GameObject/GameObject.h"
#include "../ParticleSystem/ParticleEmmiter.h"

RainMakerScript::RainMakerScript() : emmiter(nullptr)
{
}

void RainMakerScript::cleanUp()
{
}

void RainMakerScript::start()
{
	emmiter = parent->getComponet<Component::ParticleEmmiter>();
}

void RainMakerScript::update(float deltaTime)
{
	if (NOT parent OR NOT emmiter) {
		return;
	}
	emmiter->setCenter(parent->getTransform()->Position);
}
