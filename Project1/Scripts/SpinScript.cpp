#include "SpinScript.h"
#include "../GameObject/GameObject.h"
#include "../EventSystem/Handler.h"
#include "../Scene/GameScene.h"

SpinScript::SpinScript() : Component::Scripting(), axis(), rps()
{
}

SpinScript::SpinScript(Vector3 axis, Float rps) : SpinScript()
{
	this->axis = axis;
	this->rps = TWO_PI * rps * .5f;
}

void SpinScript::update(float deltaTime)
{
	parent->getLocalTransform()->rotate(axis, rps * deltaTime);
	//parent->getTransform()->Rotation *= glm::normalize(glm::quat(rps * deltaTime, axis.x, axis.y, axis.z));
}
