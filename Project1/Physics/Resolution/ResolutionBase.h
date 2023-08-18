#pragma once
namespace Component {
	class Rigidbody;
}
namespace Physics {
	class CollisionManfold;
	class Resolution {
	public:
		virtual void resolve(Component::Rigidbody* a, Component::Rigidbody* b, Physics::CollisionManfold& manafold) = 0;
		virtual void cleanUp() = 0;
	};
};
