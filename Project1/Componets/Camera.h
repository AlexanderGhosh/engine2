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
	class Cam : public ComponetBase {
	private:
		float pitch, yaw, fov;
		glm::vec3 fwd, right;
		static glm::vec3 up;
		bool inverted, perspChanged;
		Utils::Shapes::Frustrum frustrum;
		float nearPlane_dist, farPlane_dist;
	public:
		Cam();
		~Cam() = default;
		void cleanUp();
		void update(float deltaTime);
		inline Type getType() const 
		{
			return Type::Camera; 
		};

		// querys
		bool hasPerspectiveChanged() const;

		// setters
		void setFOV(Float fov);
		void setNearPlane(Float near);
		void setFarPlane(Float far);
		// getters
		Float getFOV() const;
		const glm::mat4 getViewMatrix() const;
		const glm::mat4 getPerspectiveMatrix(const glm::fvec2& screenSize);
		Vector3 getPosition() const;
	};
}

