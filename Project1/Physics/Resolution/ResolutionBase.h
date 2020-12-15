#pragma once
namespace Component {
	class RigidBody;
}
namespace Physics {
	class CollisionManfold;
	class Resolution {
	public:
		virtual void resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold) = 0;
	};
};
