#include "Engine.h"
#include <iostream>
#include <gtx/string_cast.hpp>
#include "../GameObject/GameObject.h"
#include "Resolution/ResolutionBase.h"
#include "ConstraintEngine/ConstraitnsSolver.h"
#include "../Gizmos/GizmoRenderer.h"

std::vector<Component::RigidBody*> Physics::Engine::rigidbodies = { };
std::vector<Physics::Collider*> Physics::Engine::colliders = { }; 
Physics::Resolution* Physics::Engine::resolution = nullptr; 
glm::vec3 Physics::Engine::gravity = { 0, -10, 0 };
float Physics::Engine::dampping = 1;
float Physics::Engine::deltaTime = 1.0f / 60.0f;
std::list<Gizmos::Point> Physics::Engine::tempGizmos = { };
Gizmos::Line* Physics::Engine::tempNormal = nullptr;
int Physics::Engine::roundResolution = -1;

void Physics::Engine::drawManafold(const Physics::CollisionManfold& manafold)
{
	bool adding = false;
	if (tempGizmos.size() != 4) {
		tempGizmos.resize(4);
		adding = true;
	}
	char i = 0;
	glm::vec3 mean(0);
	for (Vector3 pos : manafold.points) {
		Utils::elementAt(tempGizmos, i) = Gizmos::Point(pos, { 0, 1, 1 });
		Utils::elementAt(tempGizmos, i++).setThickness(10);
		if(adding AND i < 4)
			Gizmos::GizmoRenderer::addGizmo(&Utils::elementAt(tempGizmos, i - 1));
		mean += pos;
	}
	mean /= static_cast<float>(manafold.points.size());
	adding = NOT tempNormal;
	/*if (tempNormal)
		delete tempNormal;*/
	tempNormal = DBG_NEW Gizmos::Line(mean, mean + manafold.normal, true);
	tempNormal->setColour({ 0, 1, 1 });
	if(adding)
		Gizmos::GizmoRenderer::addGizmo(tempNormal);
}

void Physics::Engine::update()
{

	for (Component::RigidBody* rb : rigidbodies) {
		rb->intergrateForces();
	}

	auto collisions = CollisionDetection::getCollisions();

	for (Physics::CollisionManfold& manafold : collisions) {

		for (glm::vec3& p : manafold.points) {
			//p.y = manafold.bodies[0]->position->y - 0.5f;
		}
		drawManafold(manafold);
		//manafold.bodies[0]->getParent()->getRigidbody()->isKinimatic = true;
		//manafold.bodies[1]->getParent()->getRigidbody()->isKinimatic = true;
		//continue;
		Component::RigidBody* r1 = manafold.bodies[0]->getParent()->getRigidbody();
		Component::RigidBody* r2 = manafold.bodies[1]->getParent()->getRigidbody();
		r1->counter = 0;

        resolution->resolve(r1, r2, manafold);
		const float totalMass = 1.0f / (r1->getMass() + r2->getMass());
		if (NOT r1->isKinimatic) {
			r1->positionAdder(manafold.normal * manafold.penetration * (r2->getMass() * totalMass));
		}
		if (NOT r2->isKinimatic) {
			r2->positionAdder(manafold.normal * manafold.penetration * (r1->getMass() * totalMass));
		}

		// r1->isKinimatic = true;
		// r2->isKinimatic = true;
	}

	// Constraints::ConstraintsSolver::preSolveAll();

	for (Component::RigidBody* rb : rigidbodies) {

		rb->intergrateVelocity();
		rb->updateInertia();
	}
	// Constraints::ConstraintsSolver::solveAll(getDeltaTime());

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
