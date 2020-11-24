#pragma once
#include "../Collider.h"
namespace Physics {
	struct CollisionManfold;
	struct SupportPoint {
		glm::vec3 a, b, v;
		SupportPoint() : a(0), b(0), v(0) { };
		SupportPoint(const glm::vec3& a, const glm::vec3& b) : a(a), b(b), v(b - a) { };

		bool operator ==(const SupportPoint& b) {
			return v == b.v;
		}
	};
	class Narrowphase {
	public:
		virtual const CollisionManfold getCollisionData(Collider* a, Collider* b) = 0;
		SupportPoint support(const Collider* a, const Collider* b, const glm::vec3& dir);
		virtual void cleanUp() { };
	protected:
		Narrowphase() { };
	};
};