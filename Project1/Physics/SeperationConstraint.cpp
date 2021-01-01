#include "SeperationConstraint.h"

Physics::SeperationConstraint::SeperationConstraint() : Constraint()
{
	type = Constraint::Type::Seperation;
}

void Physics::SeperationConstraint::solve(const MATHS::Vector12& V, const MATHS::Matrix12& M, CollisionManfold& manafoild)
{
}
