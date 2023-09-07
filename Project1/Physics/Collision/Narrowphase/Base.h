#pragma once
#include <glm.hpp>

namespace Physics {
	class Collider;
	struct CollisionDetails;

	class Narrowphase {
	public:
		virtual const CollisionDetails getCollisionData(Collider* a, Collider* b) = 0;
		virtual void cleanUp() { };
	protected:
		Narrowphase() { };
	};
};