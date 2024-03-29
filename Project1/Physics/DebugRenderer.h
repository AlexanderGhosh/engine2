#pragma once
#include <map>
#include <list>
#include <vector>
#include <glm.hpp>

namespace Gizmos {
	class Gizmo;
}

namespace Physics {
	class Collider;
	class Simplex;

	enum class DebugType {
		Simplex,
		Collider
	};

	class DebugRenderer {
	private:
		static std::list<Gizmos::Gizmo*> allGizmos;
		// key: pointer of the object to draw
		// value: indices of the appropriate gizmos
		static std::map<std::pair<DebugType, void*>, std::list<int>> toDraw;

		static bool find(void* obj);
	public:
		static void addCollider(Physics::Collider* collider);
		static void addSimplex(Physics::Simplex* simplex, const std::vector<glm::vec3>& normals);
		static void update();

		static void cleanUp();
	};
}