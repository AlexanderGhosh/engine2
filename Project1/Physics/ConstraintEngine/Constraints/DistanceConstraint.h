#pragma once
#include "Constraints.h"
namespace Physics {
	namespace Constraints {
		class DistanceConstraint : public Constraint
		{
		public:
			DistanceConstraint(Vector3 relPosA, Vector3 relPosB, float targetLength);
			DistanceConstraint(Component::RigidBody* a, Component::RigidBody* b, Vector3 relPosA, Vector3 relPosB, float targetLength);
			void solve(const float& dt);
			void cleanUp();
			void buildJacobian();
		protected:
			const float getBias(const Physics::CollisionManfold& d) const;
			const float getLagrangian(Vector12 V, Matrix12 M, const CollisionManfold& manafoild) const;
		private:
			glm::vec3 relPosA, relPosB, globalPosA, globalPosB, ab, r1, r2;
			float targetLength;
		};
	};
};

