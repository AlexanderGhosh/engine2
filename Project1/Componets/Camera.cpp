#include "Camera.h"
#include "../GameObject/GameObject.h"

const glm::mat4 Component::Camera::getView() const
{
	return glm::lookAt(getPos(), getPos() + fwd, up);
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
	glm::vec3 front(0);
	front.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	front.y = sinf(glm::radians(pitch));
	front.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));

	fwd = glm::normalize(front);
	right = glm::normalize(glm::cross(fwd, Utils::yAxis()));
	up = glm::normalize(glm::cross(right, fwd));
}

Vector3 Component::Camera::getPos() const
{
	return parent->getTransform()->Position;
}

Float Component::Camera::getExposure() const
{
	return exposure;
}

glm::mat4 Component::Camera::getRotationMatrix() const
{
	glm::mat4 res(1);
	res = glm::rotate(res, RADIANS(yaw), Utils::yAxis());
	res = glm::rotate(res, RADIANS(pitch), Utils::xAxis());
	return res;
}

void Component::Camera::setExposure(Float expo)
{
	exposure = expo;
}
