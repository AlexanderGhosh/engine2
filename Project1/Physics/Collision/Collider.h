#pragma once
#include <glm.hpp>
namespace Physics {
	class Collider
	{
	public:
		float mass;
		glm::mat3 localInertiaTensor;
		glm::vec3 localCentroid;
		inline Collider* getCollider() { return reinterpret_cast<Collider*>(this); };
		inline void cleanUp() { };
	};
	struct AABB : public Collider {
		glm::vec3 min, max, center;
		inline AABB() : Collider(), min(0), max(0), center(0) { };
	};
};

