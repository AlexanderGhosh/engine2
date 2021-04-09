#pragma once
#include "Constraints.h"
namespace Physics {
	namespace Constraints {
		class ContactConstraint : public Constraint
		{
		private:
			CollisionManfold* collisionData;
		public:
			ContactConstraint(Component::RigidBody* a, Component::RigidBody* b);
			void solve(const float& dt);
			void cleanUp();
			void setCollisionData(CollisionManfold* data); 
			const float getLagrangian(Vector12 V, Matrix12 M, const CollisionManfold& manafoild) const; 
			const float getBias(const Physics::CollisionManfold& d) const;
			void buildJacobian();
		};
	};
};

