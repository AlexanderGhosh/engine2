#include "Engine.h"
#include <iostream>
#include <gtx/string_cast.hpp>
#include "../GameObject/GameObject.h"
#include "Resolution/ResolutionBase.h"

std::vector<Component::RigidBody*> Physics::Engine::rigidbodies = { };
std::vector<Physics::Collider*> Physics::Engine::colliders = { }; 
Physics::Resolution* Physics::Engine::resolution = nullptr;

void Physics::Engine::update()
{
	auto collisions = CollisionDetection::getCollisions();
	unsigned i = 0;
	for (Physics::CollisionManfold& manafold : collisions) {
		Component::RigidBody* b1 = manafold.bodies[0]->getParent()->getRigidbody();
		Component::RigidBody* b2 = manafold.bodies[1]->getParent()->getRigidbody();

		// std::cout << glm::to_string(manafold.normal) << " : " << std::to_string(manafold.penetration) << " GJK3D\n";
		b1->linearVelocity *= -1.0f;
		b2->linearVelocity *= -1.0f;
		return;

        resolution->resolve(b1, b2, manafold);
	}
}

void Physics::Engine::cleanUp()
{
	rigidbodies.clear();
	colliders.clear();
}
