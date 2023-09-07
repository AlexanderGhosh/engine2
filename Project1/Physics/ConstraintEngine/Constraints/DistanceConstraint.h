#pragma once
#include "Constraints.h"
namespace Physics {
	namespace Constraints {
		class DistanceConstraint : public Constraint
		{
		public:
			DistanceConstraint(Vector3 relPosA, Vector3 relPosB, float targetLength);
			DistanceConstraint(Component::Rigidbody* a, Component::Rigidbody* b, Vector3 relPosA, Vector3 relPosB, float targetLength);
			void solve(const float& dt);
			void cleanUp();
			void buildJacobian();
		private:
			glm::vec3 relPosA, relPosB;
			float targetLength;
		};
	};
};

