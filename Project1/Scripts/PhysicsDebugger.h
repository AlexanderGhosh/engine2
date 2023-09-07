#pragma once
#include "../Componets/Scripting.h"
#include "../Gizmos/GizmoRenderer.h"
#include "../Gizmos/GizmoShapes.h"

namespace Gizmos {
	class GizmoRenderer;
	class Sphere;
}
namespace Component {
	class Transform;
	class Rigidbody;
}

class PhysicsDebugger : public Component::Scripting
{
private:
	bool drawBBs, bbMade;
	Component::Transform* transform;
	Component::Rigidbody* rb;
	Gizmos::Sphere bb;
public:
	PhysicsDebugger();
	PhysicsDebugger(bool boundingBox);
	~PhysicsDebugger() = default;
	void update(float deltaTime);
	void cleanUp() override;

	void getComponents();
};
