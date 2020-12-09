#pragma once
#include <gtx/quaternion.hpp>
#include <list>
#include "Componets.h"
#include "../Physics/Collision/Collider.h"
#include "../Physics/Constraints.h"

namespace Component {
	class RigidBody : public Base
	{
	private:
		float mass, inverseMass;
		glm::mat3 globlaInverseIntertiaTensor, localInverseIntertiaTensor;
		glm::vec3 globalCentroid, localCentroid;

		glm::vec3* position;
		glm::quat* rotation;

		std::list<Physics::Collider*> colliders;
		std::list<Physics::Constraint*> constraints;

		glm::vec3 forceAccumulator, torqueAccumulator;

		void updateGlobalCentroidFromPosition();
		void updatePositionFromGlobalCentroid();

		void updateOrientation();


		const glm::vec3 localToGlobal(const glm::vec3& p) const;
		const glm::vec3 globalToLocal(const glm::vec3& p) const;
		const glm::vec3 localToGlobalVec(const glm::vec3& v) const;
		const glm::vec3 globalToLocalVec(const glm::vec3& v) const;

	public:
		glm::vec3 linearVelocity;
		glm::vec3 angularVelocity;
		bool kinematic;
		RigidBody();
		void update();
		void setParent(GameObject* parent);
		void applyForce(const glm::vec3& f, const glm::vec3& ang = Utils::zero());
		void applyAcceleration(const glm::vec3& f, const glm::vec3& at = Utils::zero());

		inline glm::mat3 getRotation() const { return glm::toMat3(*rotation); };
		inline Type getType() const { return Type::Rigidbody; };

		Utils::BigMaths::MassMatrix6 getMassMatrix() const;
		Utils::BigMaths::Vector6 getVelocities() const;
		void addCollider(Physics::Collider* collider);
		
		inline std::list<Physics::Constraint*>& getConstraints() {
			return constraints;
		}
		inline void addConstriant(Physics::Constraint* constraint) {
			if (!this)
				return;
			constraints.push_back(constraint);
		}
		inline void setVelocities(const glm::vec3& vel, const glm::vec3& ang) 
		{ 
			linearVelocity = vel; 
			angularVelocity = ang; 
		};
	};
};

