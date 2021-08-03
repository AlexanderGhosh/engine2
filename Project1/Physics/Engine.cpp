#include "Engine.h"
#include <iostream>
#include <gtx/string_cast.hpp>
#include "../GameObject/GameObject.h"
#include "Resolution/ResolutionBase.h"
#include "ConstraintEngine/ConstraitnsSolver.h"

std::vector<Component::RigidBody*> Physics::Engine::rigidbodies = { };
std::vector<Physics::Collider*> Physics::Engine::colliders = { }; 
Physics::Resolution* Physics::Engine::resolution = nullptr; 
glm::vec3 Physics::Engine::gravity = { 0, -10, 0 };
float Physics::Engine::dampping = 1;

void Physics::Engine::update()
{
	auto collisions = CollisionDetection::getCollisions();

	for (Physics::CollisionManfold& manafold : collisions) {
		Component::RigidBody* b1 = manafold.bodies[0]->getParent()->getRigidbody();
		Component::RigidBody* b2 = manafold.bodies[1]->getParent()->getRigidbody();

		if(!b1->isKinimatic)
			*manafold.bodies[0]->position -= manafold.normal * manafold.penetration;
		if (!b2->isKinimatic)
			*manafold.bodies[1]->position += manafold.normal * manafold.penetration;

        // resolution->resolve(b1, b2, manafold);
	}

	Constraints::ConstraintsSolver::preSolveAll();

	for (Component::RigidBody* rb : rigidbodies)
		rb->intergrateForces();

	Constraints::ConstraintsSolver::solveAll(getDeltaTime());

	for (Component::RigidBody* rb : rigidbodies) {
		rb->intergrateVelocity();
		rb->updateInertia();
	}
}

void Physics::Engine::cleanUp()
{
	CollisionDetection::cleanUp();
	if (resolution) {
		resolution->cleanUp();
		delete resolution;
		resolution = nullptr;
	}
	rigidbodies.clear();
	colliders.clear();
}
