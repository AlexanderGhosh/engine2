#include "PlayerControler.h"
#include "../GameObject/GameObject.h"
#include "../EventSystem/Handler.h"
#include "../Scene/GameScene.h"
#include "../Componets/Camera.h"

using EH = Events::Handler;
using Key = Events::Key;
using Cursor = Events::Cursor;
using Action = Events::Action;

PlayerControler::PlayerControler() : Component::Scripting()
{
}

void PlayerControler::update(float deltaTime) {
	Component::Transform* transform = parent->getTransform();
	Component::Camera* camera = parent->getComponet<Component::Camera>();

	glm::vec3& pos = transform->Position;

	if (EH::getKey(Key::Escape, Action::Down)) {
		parent->getScene()->close();
	}
	float speed = 10;
	if (EH::getCursor(Events::Cursor::Middle, Events::Action::Down)) {
		speed = 10;
	}
	if (EH::getKey(Key::W, Action::Down)) {
		pos += camera->getForward() * glm::vec3(1, 0, 1) * deltaTime * speed;
	}
	if (EH::getKey(Key::S, Action::Down)) {
		pos -= camera->getForward() * glm::vec3(1, 0, 1) * deltaTime * speed;
	}
	if (EH::getKey(Key::A, Action::Down)) {
		pos -= camera->getRight() * deltaTime * speed;
	}
	if (EH::getKey(Key::D, Action::Down)) {
		pos += camera->getRight() * deltaTime * speed;
	}
	if (EH::getKey(Key::Space, Action::Down)) {
		pos += Utils::yAxis() * deltaTime * speed;
	}
	if (EH::getKey(Key::L_Shift, Action::Down)) {
		pos -= Utils::yAxis() * deltaTime * speed;
	}
}
