#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
namespace Utils {
	class Camera
	{
	public:
		const glm::mat4 getView() const;
		void ProcessMouseMovement(float xOff, float yOff, bool constainPitch = 1);
		void updateCameraVectors();
		inline Camera() : pitch(0), yaw(-90), fov(45), pos(0), fwd(0, 0, -1), up(0, 1, 0), right(0) { updateCameraVectors(); };
		inline const float& getFOV() const { return fov; };
		glm::vec3 pos, fwd;
	private:
		float pitch, yaw, fov;
		glm::vec3 up, right;

	};
}

