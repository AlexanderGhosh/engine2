#pragma once
#include <glm.hpp>
#include "../../Componets/ComponetBase.h"

class SATBaseCollider;

namespace Physics {
	struct Face {
		Face() : normal(), indices() {}
		Face(Vector3 n, std::initializer_list<int> idxs) : normal(n), indices(idxs.begin(), idxs.end()) {	}
		glm::vec3 normal;
		std::vector<int> indices;
		const std::vector<glm::vec3> getFaceVertices(const std::vector<glm::vec3>& verts) const {
			std::vector<glm::vec3> res;
			for (Int i : indices) {
				res.push_back(verts[i]);
			}

		}
	};
	enum class ColliderType {
		Sphere, Cube
	};
	class Collider : public Component::ComponetBase
	{
	public:
		inline Component::Type getType() const override { return Component::Type::Collider; };

		const glm::vec3 getAbsolutePosition() const;

		void cleanUp() override;

		virtual const glm::vec3 support(Vector3 axis, bool local=false) const = 0;

		inline const float getInvMass() const { return invMass; };
		const glm::mat3 getGlobalInvInertiaTensor() const;
	protected:
		Collider();
		Collider(float mass);
		// relative to the parents origin
		glm::vec3 centerOfMass;
		float invMass;
		glm::mat3 inertiaTensor;
		std::vector<glm::vec3> allVertices;
		std::vector<Face> faces;
	};

	class SphereCollider : public Physics::Collider {
	private:
		float radius;
	public:
		SphereCollider();
		SphereCollider(float radius, float mass);

		inline Physics::ColliderType getColliderType() const override { return ColliderType::Sphere; };

		const float getRadius() const;
		const glm::vec3 support(Vector3 axis, bool local) const override;
	};

	class CubeCollider : public Collider {
	private:
		// half side length
		float width;
		const glm::quat getRotation() const;
	public:
		CubeCollider();
		CubeCollider(float width, float mass);

		inline Physics::ColliderType getColliderType() const override { return ColliderType::Cube; };

		const glm::vec3 support(Vector3 axis, bool local) const override;

	};

	struct CollisionDetails {
		bool hit, error;

		Collider* a;
		Collider* b;

		// a point on the surface which is intersecting b the most local space
		glm::vec3 a_hit;
		// a point on the surface which is intersecting a the most local space
		glm::vec3 b_hit;

		float intersection_distance;
		glm::vec3 intersection_norm;

		float coef_restitution = 1;

		// the points at which the surfaces intersect
		// std::vector< glm::vec3> intersectionManafold;
	};
};

