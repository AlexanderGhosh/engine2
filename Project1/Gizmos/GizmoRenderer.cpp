#include "GizmoRenderer.h"
#include "GizmoShapes.h"

std::unordered_map<Gizmos::Types, std::vector<Gizmos::Gizmo*>> Gizmos::GizmoRenderer::gizmos = { };
std::unordered_map<Gizmos::Types, std::vector<std::pair<glm::vec3, glm::vec3>>> Gizmos::GizmoRenderer::instanceData = { };
bool Gizmos::GizmoRenderer::updated = true;

void Gizmos::GizmoRenderer::updateInstanceDate()
{
	if (NOT updated)
		return;
	updated = false;
	for (auto itt = instanceData.begin(); itt != instanceData.end(); itt++) {
		auto& data = (*itt).second;
		const auto& type = (*itt).first;
		for (unsigned i = 0; i < data.size(); i++) {
			data[i] = { gizmos[type][i]->getPosition(), gizmos[type][i]->getColour() };
		}
	}
}

void Gizmos::GizmoRenderer::drawAll()
{
	for (auto itt = gizmos.begin(); itt != gizmos.end(); itt++) {
		for (auto gizmo : (*itt).second)
		{
			gizmo->draw();
		}
	}
}

void Gizmos::GizmoRenderer::drawType(const Types type)
{
	for (auto gizmo : gizmos[type])
	{
		gizmo->draw();
	}
}

void Gizmos::GizmoRenderer::addGizmo(Gizmo* gizmo)
{
	updated = true;
	gizmos[gizmo->getType()].push_back(gizmo);
}

void Gizmos::GizmoRenderer::cleanUp()
{
	for (auto itt = instanceData.begin(); itt != instanceData.end();) {
		(*itt).second.clear();
		itt = instanceData.erase(itt);
	}
	for (auto itt = gizmos.begin(); itt != gizmos.end();) {
		for (auto jtt = (*itt).second.begin(); jtt != (*itt).second.end();) {
			(*jtt)->cleanUp();
			jtt = (*itt).second.erase(jtt);
		}
	}
}
