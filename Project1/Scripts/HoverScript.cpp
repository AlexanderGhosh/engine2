#include "HoverScript.h"
#include "../GameObject/GameObject.h"
#include "../EventSystem/Handler.h"
#include "../Scene/GameScene.h"

HoverScript::HoverScript() : Component::Scripting(), distance(0), speed(0), moved(0)
{
}

HoverScript::HoverScript(Float dist, Float speed) : HoverScript()
{
	this->distance = dist;
	this->speed = speed;
}

void HoverScript::update(float deltaTime)
{
	//deltaTime = 1.0f / 60.0f;
	Component::Transform* transform = parent->getTransform();
	const float delta = speed * deltaTime;
	transform->Position.y += delta;
	moved += fabs(delta);
	if (moved >= distance) {
		moved = 0;
		speed *= -1;
	}
}
