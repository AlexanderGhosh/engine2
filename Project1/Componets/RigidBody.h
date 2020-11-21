#pragma once
#include <gtx/quaternion.hpp>
#include <list>
#include "Componets.h"
#include "../Physics/Collision/Collider.h"

namespace Componet {
	class RigidBody : public Base
	{
	private:
		float mass, inverseMass;
		glm::mat3 globlaInverseIntertiaTensor, localInverseIntertiaTensor;
		glm::vec3 globalCentroid, localCentroid;

		glm::vec3* position;
		glm::quat* rotation;
		glm::vec3 linearVelocity;
		glm::vec3 angularVelocity;

		std::list<Physics::Collider*> colliders;

		glm::vec3 forceAccumulator, torqueAccumulator;

		void updateGlobalCentroidFromPosition();
		void updatePositionFromGlobalCentroid();

		void updateOrientation();

		void addCollider(Physics::Collider& collider);

		const glm::vec3 localToGlobal(const glm::vec3& p) const;
		const glm::vec3 globalToLocal(const glm::vec3& p) const;
		const glm::vec3 localToGlobalVec(const glm::vec3& v) const;
		const glm::vec3 globalToLocalVec(const glm::vec3& v) const;

		void applyForce(const glm::vec3& f, const glm::vec3& at);

		inline glm::mat3 getRotation() const { return glm::toMat3(*rotation); };

	public:
		void fixedUpdate();
	};
};

