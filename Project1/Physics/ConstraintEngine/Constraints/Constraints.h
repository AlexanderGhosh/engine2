#pragma once
#include <array>
#include <glm.hpp>
#include "../../Collision/CollisionDetection.h"
#include "../../../Utils/BigMaths.h"

namespace Component {
	class RigidBody;
};
namespace Physics {
	namespace Constraints {
		enum class Type {
			None,
			Contact,
			Seperation
		};
		class Constraint {
		protected:
			BigMaths::vec12 Jacobian;
			float Bias, BaumgarteScalar;
			bool enabled;
			Type type;
			Component::RigidBody* body1, * body2;
		public:
			Constraint();
			Constraint(Component::RigidBody* a, Component::RigidBody* b);
			virtual void solve(const float& dt) = 0;
			virtual void preSolve();
			virtual void cleanUp() = 0;
			void setBodyA(Component::RigidBody* a);
			void setBodyB(Component::RigidBody* b);
			void setBodies(Component::RigidBody* a, Component::RigidBody* b);
		protected:
			virtual const float getLagrangian(Vector12 V, Matrix12 M, const CollisionManfold& manafoild) const;
			virtual const float getBias(const Physics::CollisionManfold& d) const;
			virtual void buildJacobian() = 0;
			virtual const BigMaths::vec12 getVelocityVector() const;
			virtual const BigMaths::mat12 getInvMassMatrix() const;
		};
	}
};

