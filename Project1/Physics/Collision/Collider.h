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
		glm::vec3 localCentroid; // center of mass
		inline Collider* getCollider() { return reinterpret_cast<Collider*>(this); };
		virtual const AABB* constructAABB() { return nullptr; };
		inline Component::Type getType() const { return Component::Type::Collider; };
		// inline void cleanUp() { };
		virtual glm::vec3 support(const glm::vec3& direction) const = 0;
		void setParent(GameObject* parent);
		glm::vec3* position;
		glm::quat* rotation;
		glm::vec3* scale;
	protected:
		Collider(bool add = true);
		virtual void createInertiaTensor() { };
	};
	class AABB : public Collider {
	protected:
		glm::vec3 min, max;
	public:
		inline AABB(bool add = true) : Collider(add) {
			min = glm::vec3(-0.5f);
			max = glm::vec3(0.5f);
		}
		inline AABB(const glm::vec3& min = glm::vec3(-0.5), const glm::vec3& max = glm::vec3(0.5), bool add = true) : Collider(add), min(min), max(max) { };
		inline AABB(const float& mass, const glm::vec3& centerOfMass, bool add = true) : AABB(add) { 
			this->mass = mass; 
			localCentroid = centerOfMass;
		};
		void setParent(GameObject* parent) {
			Collider::setParent(parent);
			createInertiaTensor();
		};
		const AABB* constructAABB() { return this; };
		virtual glm::vec3 support(const glm::vec3& direction) const;
		void createInertiaTensor();

		inline glm::vec3 getMin() const { return min * *scale + *position; };
		inline glm::vec3 getMax() const { return max * *scale + *position; };
		inline glm::vec3 getMinRaw() const { return min; };
		inline glm::vec3 getMaxRaw() const { return max; };


		inline void setMin(const glm::vec3& min) { this->min = min; };
		inline void setMax(const glm::vec3& max) { this->max = max; };
	};
	class BoxCollider : public AABB {
	private:
		int hi;
		AABB aabb;
	public:
		const AABB* constructAABB();
		glm::vec3 support(const glm::vec3& direction) const;
		inline BoxCollider(const float& mass, const glm::vec3& centerOfMass, bool add = true) : AABB(mass, centerOfMass, add), aabb(false), hi(0) { };
	};
};

