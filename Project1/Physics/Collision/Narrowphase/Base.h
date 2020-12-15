#pragma once
#include "../Collider.h"
namespace Physics {
	struct CollisionManfold;
	struct SupportPoint {
		glm::vec3 a, b, v, dir;
		SupportPoint() : a(0), b(0), v(0), dir(0) { }
		SupportPoint(const glm::vec3& v) : a(0), b(0), v(v), dir(0) { };
		SupportPoint(const glm::vec3& a, const glm::vec3& b) : a(a), b(b), v(b - a), dir(0) { };

		bool operator ==(const SupportPoint& b) const {
			return v == b.v;
		}
		glm::vec3 operator -(const SupportPoint& b) {
			return v - b.v;
		}
	};
	class Narrowphase {
	public:
		virtual const CollisionManfold getCollisionData(Collider* a, Collider* b) = 0;
		virtual void cleanUp() { };
	protected:
		Narrowphase() { };
	};
};