#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Componets.h"
namespace Componet {
	class Camera : public Base
	{
	public:
		const glm::mat4 getView() const;
		void ProcessMouseMovement(float xOff, float yOff, bool constainPitch = 1);
		void update();
		inline Camera() : pitch(0), yaw(-90), fov(45), pos(Utils::zero()), fwd(Utils::zAxis(-1)), up(Utils::yAxis()), right(Utils::zero()) { update(); };
		inline const float& getFOV() const { return fov; };
		inline const glm::vec3& getPos() const { return pos; };
		inline void setPos(const glm::vec3& pos) { this->pos = pos; };
		inline const glm::vec3& getForward() const { return fwd; };
	private:
		glm::vec3 pos, fwd;
		float pitch, yaw, fov;
		glm::vec3 up, right;
	};
}

