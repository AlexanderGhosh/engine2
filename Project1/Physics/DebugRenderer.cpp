#include "DebugRenderer.h"
#include "../Gizmos/GizmoShapes.h"
#include "../Gizmos/GizmoRenderer.h"
#include "Collision/Collider.h"
#include "../GameObject/GameObject.h"
#include "Collision/Narrowphase/GJK3D.h"

std::list<Gizmos::Gizmo*> Physics::DebugRenderer::allGizmos = {};

std::map<std::pair<Physics::DebugType, void*>, std::list<int>> Physics::DebugRenderer::toDraw = {};

bool Physics::DebugRenderer::find(void* obj)
{
	for (auto& [key, v] : toDraw) {
		auto& [t, o] = key;
		if (o == obj) {
			return true;
		}
	}
	return false;
}

void Physics::DebugRenderer::addCollider(Physics::Collider* collider)
{
	if (find(collider)) {
		return;
	}
	switch (collider->getColliderType())
	{
	case Physics::ColliderType::Sphere:
		Gizmos::Sphere* g = new Gizmos::Sphere(collider->getAbsolutePosition(), { 0, 1, 0 });
		toDraw[{Physics::DebugType::Collider, collider}] = {(int)allGizmos.size()};
		allGizmos.push_back(g);
		Gizmos::GizmoRenderer::addGizmo(g);
		break;
	}
}

void Physics::DebugRenderer::addSimplex(Physics::Simplex* simplex)
{
	if (find(simplex)) {
		return;
	}
	const std::vector<int> faces = {
		0, 1, 2,
		0, 3, 1,
		0, 2, 3,
		1, 3, 2
	};
	const std::vector<glm::vec3> colours = {
		{ 1, 0, 0 },
		{ 1, 1, 0 },
		{ 1, 0, 1 },
		{ 0, 1, 1 }
	};

	for (int i = 0; i < faces.size(); i += 3) {
		Vector3 p1 = (*simplex)[faces[i]];
		Vector3 p2 = (*simplex)[faces[i + 1]];
		Vector3 p3 = (*simplex)[faces[i + 2]];

		Gizmos::Triangle* t = new Gizmos::Triangle(p1, p2, p3, false);
		t->setColour(colours[i / 3]);

		toDraw[{Physics::DebugType::Simplex, simplex}].push_back(allGizmos.size());
		allGizmos.push_back(t);
		Gizmos::GizmoRenderer::addGizmo(t);
	}
}

void Physics::DebugRenderer::update()
{
	for (auto& [pair, indices] : toDraw) {
		auto& [type, obj] = pair;

		Physics::Collider* collider;
		Physics::SphereCollider* coll;
		switch (type)
		{
		case Physics::DebugType::Collider:
			collider = reinterpret_cast<Physics::Collider*>(obj);
			coll = reinterpret_cast<Physics::SphereCollider*>(collider);
			if (coll) {
				auto itt = allGizmos.begin();
				std::advance(itt, indices.front());
				Gizmos::Sphere* g = reinterpret_cast<Gizmos::Sphere*>(*itt);
				Component::Transform* t = coll->getParent()->getLocalTransform();
				g->setRadius(t->Scale.x);
				g->setPosition(t->Position);
			}
			break;
		case Physics::DebugType::Simplex:
			break;
		}
	}
}

void Physics::DebugRenderer::cleanUp()
{
	for (auto& g : allGizmos) {
		g->cleanUp();
		delete g;
		g = nullptr;
	}
	allGizmos.clear();
}
