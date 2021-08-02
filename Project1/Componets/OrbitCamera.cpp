#include "OrbitCamera.h"
#include "../GameObject/GameObject.h"
#include "../EventSystem/Handler.h"

Component::OrbitCamera::OrbitCamera() : Camera(), forward(0, 0, -1), up(0, 1, 0), sensitivity(0.25), firstMouse(true), lastPos(0),
focusPosition(0), orbitRadius(10), orbitAngle(0, 45, 0)
{
}

void Component::OrbitCamera::cleanUp()
{
	Camera::cleanUp();
}

void Component::OrbitCamera::processMouseMovement(float xOff, float yOff, bool movePos)
{
	Transform* trans = parent->getLocalTransform();
	// orbit position
	if (Events::Handler::getKey(Events::Key::L_Shift, Events::Action::Down) || movePos) {
		glm::vec3 delta = (-yOff) * up + xOff * right;
		delta *= sensitivity * 0.04f;

		focusPosition += delta;
		trans->Position += delta;
	}
	else {
		orbitAngle += sensitivity * 0.04f * glm::vec3(-yOff, xOff, 0);
	}
}

const glm::mat4 Component::OrbitCamera::getView() const
{
	return glm::lookAt(getPosition(), getPosition() + forward, up);
}

void Component::OrbitCamera::update(float deltaTime)
{
	orbitRadius -= sensitivity * Events::Handler::scrollOffset.y;
	Events::Handler::scrollOffset *= 0;
	//Utils::log(glm::to_string(Events::Handler::scrollOffset), "\r");

	const glm::quat lookRot(orbitAngle);

	forward = glm::normalize(lookRot * Utils::zAxis(-1));
	up = glm::normalize(lookRot * Utils::yAxis());
	right = glm::normalize(lookRot * Utils::xAxis(-1));

	const glm::vec3 lookPos = focusPosition - forward * orbitRadius;
	parent->getLocalTransform()->Position = lookPos;
}

Float Component::OrbitCamera::getFOV() const
{
	return Camera::getFOV();
}

Vector3 Component::OrbitCamera::getPosition() const
{
	return Camera::getPosition();
}

Float Component::OrbitCamera::getExposure() const
{
	return Camera::getExposure();
}
int cnt = 0;
void Component::OrbitCamera::mouseMove(float deltaTime)
{
	const glm::vec2& pos = Events::Handler::getCursorPos();
	if (firstMouse)
	{
		lastPos = pos;
		firstMouse = 0;
	}

	float xOff = pos.x - lastPos.x;
	float yOff = pos.y - lastPos.y;
	lastPos = pos;

	
	processMouseMovement(xOff, -yOff, 0);
}
