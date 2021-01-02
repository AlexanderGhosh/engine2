#pragma once
#include "ResolutionBase.h"
namespace Physics {
	class ImpulseBased : public Resolution
	{
	public:
		void resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold);
		void cleanUp() { };
	private:
		float getBias(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold);
		void positionalCorrection(Component::RigidBody* a, Component::RigidBody* b, const Physics::CollisionManfold& info);
	};
};

