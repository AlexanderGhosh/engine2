#pragma once
#include "ComponetBase.h"
#include "../Physics/Collision/Collider.h"

namespace Component {
	class Rigidbody : public ComponetBase
	{
	private:
		Physics::Collider* collider;
		glm::vec3* pos;
		glm::quat* rot;
		bool isKinimatic;
		bool hasGravity_;
		glm::vec3 netForce;
		glm::vec3 netTorque;
	public:
		glm::vec3 velocity;
		glm::vec3 angularVelocity;
		Rigidbody();
		Rigidbody(bool isKinimatic, bool hasGravity=true);

		void cleanUp() override;
		inline Component::Type getType() const override { return Component::Type::Rigidbody; };
		
		void setCollider(Physics::Collider& collider);
		void setParent(GameObject* parent) override;
		
		void update(float deltaTime) override;

		const float getInvMass() const;
		const glm::mat3 getGlobalInvInertiaTensor() const;

		void backPeddle(float d, glm::vec3 n);


		const glm::vec3 getVelocity() const;
		const glm::vec3 getAngularVelocity() const;


		void applyForce(Vector3 force, Vector3 pos);
		void applyGravity(Vector3 force);


		void intergrateForPosition(const float dt);
		void intergrateForVelocity(const float dt);
		inline const bool hasGravity() const { return hasGravity_; };

	};
};

