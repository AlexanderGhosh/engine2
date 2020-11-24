#pragma once
#include "../Componets/RigidBody.h"
#include "Collision/CollisionDetection.h"
namespace Physics {
	class Engine
	{
	private:
		static std::vector<Component::RigidBody*> rigidbodies;
		static std::vector<Collider*> colliders;
	public:
		static void update();
		static void cleanUp();
		static inline void addCollider(Collider* collider) {
			colliders.push_back(collider);
		};
		static inline void addRigidBody(Component::RigidBody* rigidbody) {
			rigidbodies.push_back(rigidbody);
		};
		static inline std::vector<Collider*>& getColliders() { return colliders; };
	};
};

