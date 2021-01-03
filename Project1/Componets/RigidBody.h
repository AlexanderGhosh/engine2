#pragma once
#include "Componets.h"
namespace Physics {
	class Collider;
}
namespace Component {
	struct Transform;
	class RigidBody : public Base
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
		RigidBody();
		void addCollider(Physics::Collider* collider);
		void updateInertia();
		void setParent(GameObject* parent);
		inline Type getType() const { return Type::Rigidbody; };
		void cleanUp();

		// intergration
		// for velocity
		void intergrateForces();
		// for position
		void intergrateVelocity();

		// forces
		void addForce(Vector3 force, Vector3 at);

		// getters
		Vector3 getCOS() const;
		Quaternion getRotation() const;
		glm::mat3 getRotationMatrix() const;
		Vector3 getVelocity() const;
		Vector3 getAngularVelocity() const;

		Matrix3 getInvInertia_G() const;
		const float& getInvMass() const;

		// setters

		// adders
		void velocityAdder(Vector3 add);
		void angularVelAdder(Vector3 add);
		void cosAdder(Vector3 add);
	};
};

