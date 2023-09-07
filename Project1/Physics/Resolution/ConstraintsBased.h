#pragma once
#include <list>
#include "ResolutionBase.h"
namespace Physics {
	struct Constraint;
	class ConstraintsBased : public Resolution 
	{
	public:
		ConstraintsBased();
		void resolve(Component::Rigidbody* a, Component::Rigidbody* b, Physics::CollisionManfold& manafold);
		void cleanUp();
	private:
		std::list<Constraint*> constraints;
	};
}

