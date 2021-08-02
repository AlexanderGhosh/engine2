#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "ComponetBase.h"

namespace Component {
	class Camera : public ComponetBase
	{
	protected:
		glm::vec3 fwd;
		float pitch, yaw, fov, exposure;
		glm::vec3 up, right;
	public:
		Camera();
		~Camera() = default;
		virtual void cleanUp();

		virtual const glm::mat4 getView() const;
		virtual void processMouseMovement(float xOff, float yOff, bool constainPitch = 1);
		virtual void update(float deltaTime);
		Byte_Array getByteField() const;

		// getters
		virtual Float getFOV() const;
		virtual Vector3 getPosition() const;
		Vector3 getForward() const;
		Vector3 getRight() const;
		inline Type getType() const { return Type::Camera; };
		virtual Float getExposure() const;
		glm::mat4 getRotationMatrix() const;

		// setters
		void setExposure(Float expo);
	};
}

