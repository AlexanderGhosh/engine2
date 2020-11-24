#include "Engine.h"
#include <iostream>
std::vector<Component::RigidBody*> Physics::Engine::rigidbodies = { };
std::vector<Physics::Collider*> Physics::Engine::colliders = { };

void Physics::Engine::update()
{
	auto collisions = CollisionDetection::getCollisions();
	unsigned i = 0;
	for (auto& manafold : collisions) {
		std::cout << "collision detected: " << i++ << "\r";
	}
	if(!collisions.empty())
		std::cout << std::endl;
}

void Physics::Engine::cleanUp()
{
	rigidbodies.clear();
	colliders.clear();
}
