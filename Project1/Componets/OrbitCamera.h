#pragma once
#include "Camera.h"

namespace Component {
	class OrbitCamera : public Camera
	{
	private:
		glm::vec3 focusPosition;
		float orbitRadius;
		glm::vec3 orbitAngle;

		glm::vec3 forward, up;
		glm::vec2 lastPos;
		float sensitivity;
		bool firstMouse;
	public:
		OrbitCamera();
		~OrbitCamera() = default;
		void cleanUp();

		void processMouseMovement(float xOff, float yOff, bool constainPitch = 1);
		const glm::mat4 getView() const;
		void update(float deltaTime);
		Float getFOV() const;
		Vector3 getPosition() const;
		Float getExposure() const;

		inline Type getType() const { return Type::Camera; };

		void mouseMove(float deltaTime);
	};
}
