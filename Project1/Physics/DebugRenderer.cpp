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
	Gizmos::Sphere* g;
	Gizmos::Cuboide* c;
	switch (collider->getColliderType())
	{
	case Physics::ColliderType::Sphere:
		g = new Gizmos::Sphere(collider->getAbsolutePosition(), { 0, 1, 0 });
		toDraw[{Physics::DebugType::Collider, collider}] = {(int)allGizmos.size()};
		allGizmos.push_back(g);
		Gizmos::GizmoRenderer::addGizmo(g);
		break;
	case Physics::ColliderType::Cube:
		c = new Gizmos::Cuboide(collider->getAbsolutePosition(), { 0, 1, 0 });
		c->setDimentions((glm::vec3(1)));
		toDraw[{Physics::DebugType::Collider, collider}] = { (int)allGizmos.size() };
		allGizmos.push_back(c);
		Gizmos::GizmoRenderer::addGizmo(c);
		break;
	}
}

void Physics::DebugRenderer::addSimplex(Physics::Simplex* simplex, const std::vector<glm::vec3>& normals)
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
		float s = 1;
		glm::vec3 p1 = glm::pow((*simplex)[faces[i]], glm::vec3(s));
		glm::vec3 p2 = glm::pow((*simplex)[faces[i + 1]], glm::vec3(s));
		glm::vec3 p3 = glm::pow((*simplex)[faces[i + 2]], glm::vec3(s));

		Gizmos::Triangle* t = new Gizmos::Triangle(p1, p2, p3, false);
		t->setColour(colours[i / 3]);

		toDraw[{Physics::DebugType::Simplex, simplex}].push_back(allGizmos.size());
		allGizmos.push_back(t);
		Gizmos::GizmoRenderer::addGizmo(t);

		int j = i / 3;
		if (normals.size() > j) {
			Vector3 n = normals[j];
			glm::vec3 centre = p1 + p2 + p3;
			centre /= 3.f;
			float size = .5f;
			Gizmos::Line* l = new Gizmos::Line(centre, centre + n * size, true);
			l->setColour(colours[i / 3]);

			toDraw[{Physics::DebugType::Simplex, simplex}].push_back(allGizmos.size());
			allGizmos.push_back(l);
			
			Gizmos::GizmoRenderer::addGizmo(l);

			Gizmos::Point* p = new Gizmos::Point(centre + n * size, { 0, 0, 0 });
			p->setThickness(6);

			toDraw[{Physics::DebugType::Simplex, simplex}].push_back(allGizmos.size());
			allGizmos.push_back(p);

			Gizmos::GizmoRenderer::addGizmo(p);
		}

	}
}

void Physics::DebugRenderer::update()
{
	for (auto& [pair, indices] : toDraw) {
		auto& [type, obj] = pair;

		Physics::Collider* collider;
		Physics::SphereCollider* coll_s;
		Physics::CubeCollider* coll_c;
		switch (type)
		{
		case Physics::DebugType::Collider:
			collider = reinterpret_cast<Physics::Collider*>(obj);
			switch (collider->getColliderType())
			{
			case Physics::ColliderType::Sphere:
				coll_s = reinterpret_cast<Physics::SphereCollider*>(collider);
				if (coll_s) {
					auto itt = allGizmos.begin();
					std::advance(itt, indices.front());
					Gizmos::Sphere* g = reinterpret_cast<Gizmos::Sphere*>(*itt);
					Component::Transform* t = coll_s->getParent()->getLocalTransform();
					g->setRadius(t->Scale.x);
					g->setPosition(t->Position);
				}
				break;
			case Physics::ColliderType::Cube:
				coll_c = reinterpret_cast<Physics::CubeCollider*>(collider);
				if (coll_c) {
					auto itt = allGizmos.begin();
					std::advance(itt, indices.front());
					Gizmos::Cuboide* g = reinterpret_cast<Gizmos::Cuboide*>(*itt);
					Component::Transform* t = coll_c->getParent()->getLocalTransform();
					g->setDimentions(t->Scale);
					g->setPosition(t->Position);
					g->setRotation(t->Rotation);
				}
				break;
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
