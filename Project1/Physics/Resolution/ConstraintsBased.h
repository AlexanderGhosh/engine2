#pragma once
#include "ResolutionBase.h"
namespace Physics {
	class ConstraintsBased : public Resolution 
	{
	public:
		void resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold);
	};
}

