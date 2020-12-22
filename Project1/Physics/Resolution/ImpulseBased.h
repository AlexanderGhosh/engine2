#pragma once
#include "ResolutionBase.h"
namespace Physics {
	class ImpulseBased : public Resolution
	{
	public:
		void resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold);
	private:
		float getImpuseForce(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& info);
	};
};

