#include "Camera.h"
#include <iostream>
#include <gtx/string_cast.hpp>
#include "../GameObject/GameObject.h"

const glm::mat4 Component::Camera::getView() const
{
	return glm::lookAt(pos, pos + fwd, up);
}
void Component::Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
	xOffset *= 0.25;
	yOffset *= 0.25;

	yaw += xOffset;
	pitch += yOffset;// up down

	if (constrainPitch) {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		else if (pitch < -89.0f) {
			pitch = -89.0f;
		}
	}
	this->update(0);
}
void Component::Camera::update(float deltaTime) {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	fwd = glm::normalize(front);
	right = glm::normalize(glm::cross(fwd, Utils::yAxis()));
	up = glm::normalize(glm::cross(right, fwd));
}






glm::vec3 Component::Cam::up = Utils::yAxis();

Component::Cam::Cam() : right(Utils::xAxis()), fwd(Utils::zAxis()), fov(45.0f), inverted(false), pitch(0), yaw(-90.0f), frustrum(), perspChanged(true), nearPlane_dist(0.1f), farPlane_dist(100.0f)
{
}

void Component::Cam::cleanUp()
{
}

void Component::Cam::update(float deltaTime)
{
	fwd.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	fwd.y = sin(glm::radians(pitch));
	fwd.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	fwd = Utils::normalize(fwd);
	right = Utils::normalize(glm::cross(fwd, up));
}

bool Component::Cam::hasPerspectiveChanged() const
{
	return perspChanged;
}

void Component::Cam::setFOV(Float fov)
{
	this->fov = fov;
	perspChanged = true;
}

void Component::Cam::setNearPlane(Float near)
{
	nearPlane_dist = near;
	perspChanged = true;
}

void Component::Cam::setFarPlane(Float far)
{
	farPlane_dist = far;
	perspChanged = true;
}

Float Component::Cam::getFOV() const
{
	return fov;
}

const glm::mat4 Component::Cam::getViewMatrix() const
{
	Vector3 pos = getPosition();
	return glm::lookAt(pos, pos + fwd, up);
}

const glm::mat4 Component::Cam::getPerspectiveMatrix(const glm::fvec2& screenSize)
{
	perspChanged = false;
	return glm::perspective(glm::radians(fov), screenSize.x / screenSize.y, farPlane_dist, farPlane_dist);
}

Vector3 Component::Cam::getPosition() const
{
	return parent->getTransform()->Position;
}
