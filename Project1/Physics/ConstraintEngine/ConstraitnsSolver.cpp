#include "ConstraitnsSolver.h"
#include "Constraints/Constraints.h"
#include "../../GameObject/GameObject.h"

std::list <Physics::Constraints::Constraint*> Physics::Constraints::ConstraintsSolver::constraints = { };

void Physics::Constraints::ConstraintsSolver::cleanUp()
{
	for (auto itt = constraints.begin(); itt != constraints.end();) {
		(*itt)->cleanUp();
		// delete* itt;
		itt = constraints.erase(itt);
	}
}

void Physics::Constraints::ConstraintsSolver::addConstraint(Constraint* c)
{
	constraints.push_back(c);
}
void Physics::Constraints::ConstraintsSolver::solveAll(const float& dt)
{
	for (Constraint*& c : constraints) {
		c->solve(dt);
	}
}

void Physics::Constraints::ConstraintsSolver::preSolveAll()
{
	for (Constraint*& c : constraints) 
		c->preSolve();
	
}

Physics::Constraints::Constraint* Physics::Constraints::ConstraintsSolver::popConstraint()
{
	Constraint* back = constraints.back();
	constraints.pop_back();
	return back;
}
