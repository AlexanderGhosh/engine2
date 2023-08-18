#pragma once
#include <glm.hpp>
#include "../../Componets/ComponetBase.h"

namespace Physics {
	class Collider : public Component::ComponetBase
	{
	public:
	protected:
		Collider();
	};

	class SphereCollider : public Component::ComponetBase {
	private:
		float radius;
		// relative to the parents origin
		glm::vec3 centerOfMass;
		float invMass;
		glm::mat3 inertiaTensor;
	public:
		SphereCollider();
		SphereCollider(float radius, float mass);
		void setParent(GameObject* parent) override;

		void cleanUp() override;
		inline Component::Type getType() const override { return Component::Type::Collider; };

		const glm::vec3 getAbsolutePosition() const;

		inline const float getRadius() const { return radius; };
		inline const float getInvMass() const { return invMass; };
		const glm::mat3 getGlobalInvInertiaTensor() const;
	};

	struct CollisionDetails {
		SphereCollider* a;
		SphereCollider* b;

		// a point on the surface which is intersecting b the most relative the a's centre
		glm::vec3 a_hit;
		// a point on the surface which is intersecting a the most relative the b's centre
		glm::vec3 b_hit;

		float intersection_distance;
		glm::vec3 intersection_norm;

		// the points at which the surfaces intersect
		// std::vector< glm::vec3> intersectionManafold;
	};
};

