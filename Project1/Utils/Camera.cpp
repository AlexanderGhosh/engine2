#include "Camera.h"
#include <iostream>
#include <gtx/string_cast.hpp>

const glm::mat4 Utils::Camera::getView() const
{
	return glm::lookAt(pos, pos + fwd, up);
}
void Utils::Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
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
	// this->position = glm::vec3(0);

	this->updateCameraVectors();
}
void Utils::Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	fwd = glm::normalize(front);
	right = glm::normalize(glm::cross(fwd, { 0, 1, 0 }));
	up = glm::normalize(glm::cross(right, fwd));
}