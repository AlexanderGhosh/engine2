#include "CubeCollider.h"

CubeCollider::CubeCollider() : SATBaseCollider()
{
	vertices = {
		{ -0.5, -0.5, -0.5 },
		{  0.5, -0.5, -0.5 },
		{  0.5,  0.5, -0.5 },
		{ -0.5,  0.5, -0.5 },

		{ -0.5, -0.5,  0.5 },
		{  0.5, -0.5,  0.5 },
		{  0.5,  0.5,  0.5 },
		{ -0.5,  0.5,  0.5 }
	};

	normals = {
		{ 0, 1, 0 },
		{ 1, 0, 0 },
		{ 0, 0, 1 },

		{ 0, -1, 0 },
		{ -1, 0, 0 },
		{ 0, 0, -1 },
	};
}

CubeCollider::CubeCollider(float mass) : CubeCollider()
{
	invMass = 1.f / mass;
}

const std::list<glm::vec3> CubeCollider::getGlobalVertices() const
{
	std::list<glm::vec3> res;
	const glm::mat4 model = transform->getModel();

	for (Vector3 v : vertices) {
		glm::vec4 a = glm::vec4(v, 1);
		a = model * a;
		res.push_back(glm::vec3(a));
	}
	return res;
}

const std::list<glm::vec3> CubeCollider::getGlobalNormals() const
{
	std::list<glm::vec3> res;
	const glm::mat3 rot = glm::toMat3(transform->Rotation);

	for (Vector3 n : normals) {
		res.push_back(n * rot);
	}
	return res;
}
