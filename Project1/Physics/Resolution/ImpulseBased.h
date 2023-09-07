#pragma once
#include "ResolutionBase.h"
namespace Physics {
	class ImpulseBased : public Resolution
	{
	public:
		void resolve(Component::Rigidbody* a, Component::Rigidbody* b, Physics::CollisionManfold& manafold);
		void cleanUp() { };
	private:
		float getBias(Component::Rigidbody* a, Component::Rigidbody* b, Physics::CollisionManfold& manafold);
		void positionalCorrection(Component::Rigidbody* a, Component::Rigidbody* b, const Physics::CollisionManfold& info);
	};
};

