#pragma once
#include <array>
#include <glm.hpp>
#include "Collision/CollisionDetection.h"
namespace Physics {
#define BETA 1.0f
#define FPS 60.0f
#define MATHS Utils::BigMaths
	struct Constraint {
		MATHS::Vector12 Jacobian;
		bool enabled;
		Constraint() : Jacobian(), enabled(1), type(Type::Contact) { 
		};
		virtual void solve(const MATHS::Vector12& V, const MATHS::Matrix12& M, CollisionManfold& manafoild);
		enum class Type {
			Contact
		};
		Type type;
	private:
		virtual const float getLagrangian(const MATHS::Vector12& V, const MATHS::Matrix12& M, const CollisionManfold& manafoild) const;
		const float getBias(const Physics::CollisionManfold& d) const;
	};
};

