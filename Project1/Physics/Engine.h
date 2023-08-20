#pragma once
#include <list>
#include <vector>
#include "../Utils/General.h"
#include "Collision/Narrowphase/Base.h"

namespace Component {
	class Rigidbody;
}

class SATBaseCollider;
namespace Physics {
	class Collider;
	struct CollisionDetails;

	class Engine
	{
	private:
		static std::list<Component::Rigidbody*> rigidbodies;
		static std::list<Physics::Collider*> colliders;

		static std::vector<Physics::CollisionDetails> getIntersections();
		/// <summary>
		/// For each collision both objects will be moved along there velocity axis inorder to resolve overlap, then the impuse force is applyed to the sigular point of collision
		/// </summary>
		/// <param name="details"></param>
		static void resolveIntersections(const std::vector<Physics::CollisionDetails>& details);
	public:
		static bool RenderDebug;
		static Physics::Narrowphase* narrowphase;
		static glm::vec3 gravity;
		static void update();
		static void cleanUp();

		static void addRigidbody(Component::Rigidbody* rb);
		static void addCollider(Physics::Collider* col);

		static glm::vec3 calcImpulseForce(Component::Rigidbody* a, Component::Rigidbody* b, Vector3 norm, Float coef_restitution);

		// template<class T>
		// static inline void setResponse() { resolution = DBG_NEW T(); };
	};
}

