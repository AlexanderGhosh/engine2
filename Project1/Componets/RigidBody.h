#pragma once
#include "Componets.h"
namespace Physics {
	namespace Constraints {
		class Constraint;
	}
	class Collider;
}
namespace Component {
	struct Transform;
	class RigidBody : public ComponetBase
	{
	private:
		Transform* transform;
		// Center of Mass
		glm::vec3 cos;
		glm::quat* rotation;
		glm::vec3 velocity, angularVelocity;
		glm::vec3 force, torque;
		float mass, invMass;
		glm::mat3 l_inertia, l_invInertia;
		glm::mat3 g_inertia, g_invInertia;
		std::list<Physics::Collider*> colliders;

		void clearForces();
	public:
		bool isKinimatic, hasGravity;
	public:
		std::vector<Physics::Constraints::Constraint*> constraints;
		int counter = 0;
		void update(float deltaTime) {
			/*if(NOT isKinimatic)
				Utils::Log(":::::::::" + std::to_string(counter++) + ":::::" + Utils::to_string_precision(transform->Position.y, -1));*/
		}
		RigidBody();
		void addCollider(Physics::Collider* collider);
		void updateInertia();
		void setParent(GameObject* parent);
		inline Type getType() const { return Type::Rigidbody; };
		void cleanUp();

		// intergration
		// for velocity adds gravity and dampening
		void intergrateForces();
		// for position
		void intergrateVelocity();

		// forces
		void addForce(Vector3 force, Vector3 at);

		// getters
		Vector3 getCOM() const;
		Quaternion getRotation() const;
		glm::mat3 getRotationMatrix() const;
		const glm::vec3 getVelocity(bool zeroKini = false) const;
		const glm::vec3 getAngularVelocity(bool zeroKini = false) const;

		const glm::mat3 getInvInertia_G(bool zeroKini = false) const;
		const float getInvMass(bool zeroKini = false) const;
		const float getMass(bool zeroKini = false) const;

		// setters

		// adders
		void positionAdder(Vector3 add);
		void velocityAdder(Vector3 add);
		void angularVelAdder(Vector3 add);
		void cosAdder(Vector3 add);
	};
};

