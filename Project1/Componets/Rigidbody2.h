#pragma once
#include "Componets.h"
namespace Physics {
	class Collider;
}
namespace Component {
	struct Transform;
	class Rigidbody2 : public Base
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

		// intergration
		// for velocity
		void intergrateForces();
		// for position
		void intergrateVelocity();

		void clearForces();
	public:
		Rigidbody2();
		void addCollider(Physics::Collider* collider);
		void update();
		void setParent(GameObject* parent);
		inline Type getType() const { return Type::Rigidbody; };

		// forces
		void addForce(Vector3 force, Vector3 at);

	};
};

