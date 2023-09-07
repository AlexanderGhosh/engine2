#include "PhysicsDebugger.h"
#include "../GameObject/GameObject.h"
#include "../Componets/ComponetBase.h"
#include "../Componets/RigidBody.h"

PhysicsDebugger::PhysicsDebugger() : drawBBs(false), transform(0), rb(0), bb(), bbMade(false)
{
}

PhysicsDebugger::PhysicsDebugger(bool boundingBox) : PhysicsDebugger()
{
	drawBBs = boundingBox;
}

void PhysicsDebugger::update(float deltaTime)
{
	getComponents();
	if (bbMade) {
		bb.setRadius(transform->Scale.x);
		bb.setPosition(transform->Position);
	}
}

void PhysicsDebugger::cleanUp()
{
	Scripting::cleanUp();
	bb.cleanUp();
}

void PhysicsDebugger::getComponents()
{
	if (!transform) {
		transform = parent->getLocalTransform();
	}
	if (!rb) {
		rb = parent->getComponet<Component::Rigidbody>();
	}

	if (transform && rb && drawBBs && !bbMade) {
		bbMade = true;
		bb = Gizmos::Sphere(transform->Position, glm::vec3(0, 1, 0));
		Gizmos::GizmoRenderer::addGizmo(&bb);
	}
}
