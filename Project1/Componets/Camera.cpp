#include "Camera.h"
#include "../GameObject/GameObject.h"

Component::Camera::Camera() : pitch(0), yaw(-90), fov(45), pos(Utils::zero()), fwd(Utils::zAxis(-1)), up(Utils::yAxis()), right(Utils::zero()), exposure(0.1f) 
{
	update(0); 
}

void Component::Camera::cleanUp()
{
}

const glm::mat4 Component::Camera::getView() const
{
	return glm::lookAt(getPosition(), getPosition() + fwd, up);
}

void Component::Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
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
	update(0);
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

Float Component::Camera::getFOV() const
{
	return fov;
}

Vector3 Component::Camera::getPosition() const
{
	return parent->getLocalTransform()->Position;
}

Vector3 Component::Camera::getForward() const
{
	return fwd;
}

Vector3 Component::Camera::getRight() const
{
	return right;
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
