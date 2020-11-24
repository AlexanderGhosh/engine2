#pragma once
#include <glm.hpp>
#include "../../Componets/Componets.h"
namespace Physics {
	struct AABB;
	class Collider : public Component::Base
	{
	public:
		float mass;
		glm::mat3 localInertiaTensor;
		glm::vec3 localCentroid;
		inline Collider* getCollider() { return reinterpret_cast<Collider*>(this); };
		virtual const AABB* constructAABB() const { return nullptr; };
		inline Component::Type getType() const { return Component::Type::Collider; };
		// inline void cleanUp() { };
		virtual glm::vec3 support(const glm::vec3& direction) const = 0;
	protected:
		Collider();
	};
	struct AABB : public Collider {
		glm::vec3 min, max, center;
		inline AABB() : Collider(), min(0), max(0), center(0) { };
		inline AABB(const glm::vec3& min, const glm::vec3& max, const glm::vec3& center) : Collider(), min(min), max(max), center(center) { };
		const AABB* constructAABB() const { return this; };
		const glm::vec3 getCenter() const { return Base::getCenter() + center; };
		glm::vec3 support(const glm::vec3& direction) const;
	};
};

