#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Componets.h"
namespace Component {
	class Camera : public ComponetBase
	{
	public:
		const glm::mat4 getView() const;
		void ProcessMouseMovement(float xOff, float yOff, bool constainPitch = 1);
		void update(float deltaTime);
		inline Camera() : pitch(0), yaw(-90), fov(45), pos(Utils::zero()), fwd(Utils::zAxis(-1)), up(Utils::yAxis()), right(Utils::zero()) { update(0); };
		~Camera() = default;
		void cleanUp() { };

		// getters
		inline const float& getFOV() const { return fov; };
		inline Vector3 getPos() const { return pos; };
		inline Vector3 getForward() const { return fwd; };
		inline Vector3 getRight() const { return right; };
		inline Type getType() const { return Type::Camera; };
		// setters
		inline void setPos(const glm::vec3& pos) { this->pos = pos; };
	private:
		glm::vec3 pos, fwd;
		float pitch, yaw, fov;
		glm::vec3 up, right;
	};
}

