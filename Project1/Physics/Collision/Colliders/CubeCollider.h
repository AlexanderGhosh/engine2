#pragma once
#include "SATBaseCollider.h"

class CubeCollider : public SATBaseCollider {
private:
public:
	CubeCollider();
	CubeCollider(float mass);

	const std::list<glm::vec3> getGlobalVertices() const override;
	const std::list<glm::vec3> getGlobalNormals() const override;
};