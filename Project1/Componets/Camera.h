#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Componets.h"

namespace Component {
	class Camera : public ComponetBase
	{
	private:
		glm::vec3 pos, fwd;
		float pitch, yaw, fov, exposure;
		glm::vec3 up, right;
	public:
		Camera();
		~Camera() = default;
		void cleanUp();

		const glm::mat4 getView() const;
		void processMouseMovement(float xOff, float yOff, bool constainPitch = 1);
		void update(float deltaTime);

		// getters
		Float getFOV() const;
		Vector3 getPosition() const;
		Vector3 getForward() const;
		Vector3 getRight() const;
		inline Type getType() const { return Type::Camera; };
		Float getExposure() const;
		glm::mat4 getRotationMatrix() const;

		// setters
		void setExposure(Float expo);
	};
}

