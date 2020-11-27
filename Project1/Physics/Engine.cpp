#include "Engine.h"
#include <iostream>
#include "../GameObject/GameObject.h"
std::vector<Component::RigidBody*> Physics::Engine::rigidbodies = { };
std::vector<Physics::Collider*> Physics::Engine::colliders = { };

void Physics::Engine::update()
{
	auto collisions = CollisionDetection::getCollisions();
	unsigned i = 0;
	for (Physics::CollisionManfold& manafold : collisions) {
		Component::RigidBody* b1 = manafold.bodies[0]->getParent()->getRigidbody();
		Component::RigidBody* b2 = manafold.bodies[1]->getParent()->getRigidbody();
		if ((unsigned)b1 + (unsigned)b2 == 0)
			continue;

		auto& c_1 = b1->getConstraints();
		auto& c_2 = b2->getConstraints();
		for (int i = 0; i < c_1.size(); i++) {
			Constraint* c = Utils::elementAt(c_1, i);
			if (Utils::find(c_2, c)) {
				// std::cout << "collision detected: " << i++ << "\r";
				Utils::BigMaths::Vector12 V;
				Utils::BigMaths::Matrix12 M;

				V = Utils::BigMaths::combine(b1->getVelocities(), b2->getVelocities());
				M = Utils::BigMaths::combine(b1->getMassMatrix(), b2->getMassMatrix());
				const Utils::BigMaths::Vector12 delta = c->getDeltaV(V, M, manafold);
				b1->applyForce({ delta[0], delta[1], delta[2] }, { delta[3], delta[4], delta[5] });
				b2->applyForce({ delta[6], delta[7], delta[8] }, { delta[9], delta[10], delta[11] });
			}
		}
		for (Component::RigidBody* rb : rigidbodies) {
			// rb->update();
		}

	}
	// if(!collisions.empty())
		// std::cout << std::endl;
}

void Physics::Engine::cleanUp()
{
	rigidbodies.clear();
	colliders.clear();
}
