#pragma once
#include "ComponetBase.h"
#include "../Physics/Collision/Collider.h"

namespace Component {
	class Rigidbody : public ComponetBase
	{
	private:
		Physics::Collider* collider;
		glm::vec3* pos;
		bool isKinimatic;
	public:
		glm::vec3 velocity;
		glm::vec3 angularVelocity;
		Rigidbody();
		Rigidbody(bool isKinimatic);

		void cleanUp() override;
		inline Component::Type getType() const override { return Component::Type::Rigidbody; };
		
		void setCollider(Physics::Collider& collider);
		void setParent(GameObject* parent) override;
		
		void update(float deltaTime) override;

		const float getInvMass() const;
		const glm::mat3 getGlobalInvInertiaTensor() const;

		void backPeddle(float d);


		void applyForce(glm::vec3 force);


		void intergrateForPosition(const float dt);
	};
};

