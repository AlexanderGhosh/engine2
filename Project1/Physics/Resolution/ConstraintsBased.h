#pragma once
#include <list>
#include "ResolutionBase.h"
namespace Physics {
	struct Constraint;
	class ConstraintsBased : public Resolution 
	{
	public:
		ConstraintsBased();
		void resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold);
		void cleanUp();
	private:
		std::list<Constraint*> constraints;
	};
}

