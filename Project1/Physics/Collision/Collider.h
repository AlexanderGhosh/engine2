#pragma once
#include <glm.hpp>
#include "../../Componets/ComponetBase.h"

class SATBaseCollider;

namespace Physics {
	enum class ColliderType {
		Sphere, Cube
	};
	class Collider : public Component::ComponetBase
	{
	public:
		inline Component::Type getType() const override { return Component::Type::Collider; };
		virtual inline Physics::ColliderType getColliderType() const = 0;

		const glm::vec3 getAbsolutePosition() const;

		void cleanUp() override;

		virtual const glm::vec3 support(Vector3 axis) const = 0;

		virtual const float getRadius(float theta) const = 0;
		inline const float getInvMass() const { return invMass; };
		const glm::mat3 getGlobalInvInertiaTensor() const;
	protected:
		Collider();
		Collider(float mass);
		// relative to the parents origin
		glm::vec3 centerOfMass;
		float invMass;
		glm::mat3 inertiaTensor;
	};

	class SphereCollider : public Physics::Collider {
	private:
		float radius;
	public:
		SphereCollider();
		SphereCollider(float radius, float mass);

		inline Physics::ColliderType getColliderType() const override { return ColliderType::Sphere; };

		const float getRadius(float theta) const override;
		const glm::vec3 support(Vector3 axis) const override;
	};

	/*class CubeCollider : public Collider {
	private:
		float width;
	public:
		CubeCollider();
		CubeCollider(float width, float mass);
		const float getRadius(float theta) const override;


	};*/

	struct CollisionDetails {
		Collider* a;
		Collider* b;

		// a point on the surface which is intersecting b the most world space
		glm::vec3 a_hit;
		// a point on the surface which is intersecting a the most world space
		glm::vec3 b_hit;

		float intersection_distance;
		glm::vec3 intersection_norm;

		float coef_restitution = 1;

		// the points at which the surfaces intersect
		// std::vector< glm::vec3> intersectionManafold;
	};
};

