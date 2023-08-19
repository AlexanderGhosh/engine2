#pragma once
#include "../Collider.h"

class SATBaseCollider : public Physics::Collider {
protected:
	SATBaseCollider();
	std::list<glm::vec3> vertices;
	std::list<glm::vec3> normals;

	float invMass;
	glm::mat3 inertiaTensor;
	Component::Transform* transform;
public:
	void setParent(GameObject* obj) override;
	void cleanUp() override;

	inline Component::Type getType() const override { return Component::Type::Collider; };
	inline const float getInvMass() const { return invMass; };
	const glm::mat3 getGlobalInvInertiaTensor() const;

	virtual std::array<float, 2> project(Vector3 axis) const;
	virtual const std::list<glm::vec3> getGlobalVertices() const = 0;
	virtual const std::list<glm::vec3> getGlobalNormals() const = 0;
};