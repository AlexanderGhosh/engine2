#pragma once
#include <glm.hpp>
#include <vector>
#include <unordered_map>


namespace Gizmos {
	class Gizmo;
	enum class Types;
	class GizmoRenderer
	{
	private:
		static std::unordered_map<Types, std::vector<Gizmo*>> gizmos;
		static std::unordered_map<Types, std::vector<std::pair<glm::vec3, glm::vec3>>> instanceData;
		static bool updated;
		static void updateInstanceDate();
	public:
		static void init();
		static void drawAll();
		static void drawType(const Types type);
		static void addGizmo(Gizmo* gizmo);
		static void cleanUp();
	};
}

