#pragma once
#include "Constraints.h"
namespace Physics {
	struct SeperationConstraint : public Constraint
	{
		SeperationConstraint();
		void solve(const MATHS::Vector12& V, const MATHS::Matrix12& M, CollisionManfold& manafoild);
	};
}

