#pragma once
#include "Base.h"
#include "../../../Utils/Shapes.h"

//namespace Utils {
//	namespace Shapes {
//		template<class T>
//		class Edge;
//		template<class T, char size>
//		class Face;
//	}
//}
namespace Physics {
	class SAT3D : public Narrowphase {
	private:
		using Projection = std::array<float, 2>;
		using ClippedPoints = std::vector<glm::vec3>;
		bool overlap(const Projection& a, const Projection& b) const;
		float getOverlap(const Projection& a, const Projection& b) const;
		void testNormals(ColliderSAT*& aSAT, ColliderSAT*& bSAT, std::list<glm::vec3>& normals, CollisionManfold& res) const;
		Utils::Shapes::Face<glm::vec3, 4> bestFace(ColliderSAT* coll, const glm::vec3& n) const;
		ClippedPoints getContactData(CollisionManfold& info) const;
		ClippedPoints clip(Utils::Shapes::Face<glm::vec3, 4>& ref, ClippedPoints inc) const;
	public:
		const CollisionManfold getCollisionData(Collider* a, Collider* b);
	};
}

