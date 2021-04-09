#include "ConstraintsBased.h"
#include "../ConstraintEngine/Constraints/ContactConstraint.h"
#include "../../Componets/Rigidbody.h"
#include "../ConstraintEngine/ConstraitnsSolver.h"

Physics::ConstraintsBased::ConstraintsBased() : Resolution(), constraints()
{
}

void Physics::ConstraintsBased::resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold)
{
	Constraints::ContactConstraint contact = Constraints::ContactConstraint(a, b);

	contact.preSolve();
	contact.setCollisionData(&manafold);
	contact.solve(1); 

	Constraints::ConstraintsSolver::popConstraint();
	contact.cleanUp();
}

void Physics::ConstraintsBased::cleanUp()
{
	/*for (auto itt = constraints.begin(); itt != constraints.end();) {
		(*itt)->cleanUp();
		delete* itt;
		itt = constraints.erase(itt);
	}*/
}
