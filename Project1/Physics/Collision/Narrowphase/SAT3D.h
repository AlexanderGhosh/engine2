#pragma once
#include "Base.h"
namespace Physics {
	class SAT3D : public Narrowphase {
		using Projection = std::array<float, 2>;
	private:
		bool overlap(const Projection& a, const Projection& b) const;
		float getOverlap(const Projection& a, const Projection& b) const;
		void testNormals(ColliderSAT*& aSAT, ColliderSAT*& bSAT, std::list<glm::vec3>& normals, CollisionManfold& res) const;
		void bestEdge(ColliderSAT* coll, const glm::vec3& n) const;
	public:
		const CollisionManfold getCollisionData(Collider* a, Collider* b);
	};
}

