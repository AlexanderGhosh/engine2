#pragma once
#include "ComponetBase.h"

namespace Component {
	class CharacterController : public ComponetBase
	{
	private:
		glm::vec3 velocity;
		glm::vec3* position;
		bool isGrounded;
		/// <summary>
		/// intergrates velocity to position and adds gravity with sourced from the physics engine
		/// </summary>
		void intergrateVelocity(Float deltaTime);
	public:
		bool hasGravity;
		CharacterController();
		~CharacterController() = default;
		inline Type getType() const { return Component::Type::CharacterController; };
		void cleanUp();
		Byte_Array getByteField() const;

		// getters
		Vector3 getVelocity() const;
		Vector3 getPosition() const;

		// setters
		void setParent(GameObject* parent);
		void setVelocity(Vector3 vel);
		void setPosition(Vector3 pos);
		void setIsGrounded(const bool grounded);

		// adders
		void addVelocity(Vector3 vel);
		void addPosition(Vector3 pos);

		// events
		void update(float deltaTime);
	};
};

