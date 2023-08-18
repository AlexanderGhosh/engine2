#pragma once
#include <array>
#include <glm.hpp>
#include "../../Collision/CollisionDetection.h"
namespace Component {
	class Rigidbody;
};
namespace Physics {
#define MATHS Utils::BigMaths
	namespace Constraints {
		enum class Type {
			None,
			Contact,
			Seperation
		};
		class Constraint {
		protected:
			MATHS::Vector12 Jacobian;
			bool enabled;
			Type type;
			Component::Rigidbody* body1, * body2;
		public:
			Constraint();
			Constraint(Component::Rigidbody* a, Component::Rigidbody* b);
			virtual void solve(const float& dt) = 0;
			virtual void preSolve();
			virtual void cleanUp() = 0;
			void setBodyA(Component::Rigidbody* a);
			void setBodyB(Component::Rigidbody* b);
			void setBodies(Component::Rigidbody* a, Component::Rigidbody* b);
		protected:
			virtual const float getLagrangian(const MATHS::Vector12& V, const MATHS::Matrix12& M, const CollisionManfold& manafoild) const;
			virtual const float getBias(const Physics::CollisionManfold& d) const;
			virtual void buildJacobian() = 0;
		};
	}
};

