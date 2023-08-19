#include "PlayerControler.h"
#include "../GameObject/Terrain.h"
#include "../GameObject/GameObject.h"
#include "../EventSystem/Handler.h"
#include "../Scene/GameScene.h"
#include "../Componets/Camera.h"
#include "../Componets/CharacterController.h"

using EH = Events::Handler;
using Key = Events::Key;
using Cursor = Events::Cursor;
using Action = Events::Action;

PlayerControler::PlayerControler() : Component::Scripting(), firstMouse(true), lastX(0), lastY(0), yOffset(1.5f)
{
}

void PlayerControler::update(float deltaTime) {
	Component::Transform* transform = parent->getLocalTransform();
	Component::Camera* camera = parent->getComponet<Component::Camera>();
	Component::CharacterController* cc = parent->getComponet<Component::CharacterController>();

	glm::vec3& pos = transform->Position;
	Vector3 fwd = camera->getForward();

	if (EH::getKey(Key::Escape, Action::Down)) {
		parent->getScene()->close();
	}
	const float baseSpeed = 3;
	const float speedMultiplyer = 6;
	float speed = baseSpeed * deltaTime;
	if (EH::getCursor(Events::Cursor::Middle, Events::Action::Down)) {
		speed = speedMultiplyer * baseSpeed * deltaTime;
	}
	if (EH::getKey(Key::W, Action::Down)) {
		pos += fwd * glm::vec3(1, 0, 1) * speed;
	}
	if (EH::getKey(Key::S, Action::Down)) {
		pos -= fwd * glm::vec3(1, 0, 1) * speed;
	}
	if (EH::getKey(Key::A, Action::Down)) {
		pos -= camera->getRight() * speed;
	}
	if (EH::getKey(Key::D, Action::Down)) {
		pos += camera->getRight() * speed;
	}
	const bool grounded = isGrounded(deltaTime);
	cc->setIsGrounded(grounded);
	if (EH::getKey(Key::Space, Action::Down)) {
		pos += Utils::yAxis() * speed;
		// cc->addVelocity(Utils::yAxis(10));
	}
	if (EH::getKey(Key::L_Shift, Action::Down)) {
		pos -= Utils::yAxis() * speed;
	}
}

void PlayerControler::mouseMove(float deltaTime)
{
	const glm::vec2& pos = EH::getCursorPos();
	if (firstMouse)
	{
		lastX = pos.x;
		lastY = pos.y;
		firstMouse = 0;
	}

	float xoffset = pos.x - lastX;
	float yoffset = lastY - pos.y;
	lastX = pos.x;
	lastY = pos.y;

	Component::Camera* camera = parent->getComponet<Component::Camera>();
	camera->processMouseMovement(xoffset, yoffset);
}

bool PlayerControler::isGrounded(float dt)
{
	return false;
	if (dt == 0)
		dt = 1;
	Terrain currentGround = ground[0];
	Component::CharacterController* cc = parent->getComponet<Component::CharacterController>();
	for (Terrain land : ground) {
		Vector3 landPos = land.getPosition();
		glm::vec3 landScale = land.getScale() / 2.0f;
		Vector3 pos = cc->getPosition();
		if (landPos.x - landScale.x < pos.x AND landPos.x + landScale.x > pos.x) {
			if (landPos.z - landScale.z < pos.z AND landPos.z + landScale.z > pos.z) {
				currentGround = land;
				break;
			}
		}
	}
	float h = currentGround.getHeight(cc->getPosition().x, cc->getPosition().z, false);
	h += currentGround.getPosition().y;
	float y = cc->getPosition().y - yOffset;
	if (y <= h) {
		glm::vec3 p = cc->getPosition();
		float delta = (h + yOffset) - p.y;
		if (dt == 0) {
			int i = 0;
		}
		cc->addPosition(Utils::yAxis(delta));
		return true;
	}
	return false;
}
