#include "Collider.h"
#include "../Engine.h"

glm::vec3 Physics::AABB::support(const glm::vec3& direction) const
{
	glm::vec3 res(0);//   max             min
	for (char i = 0; i < 3; i++) 
		res[i] = (direction[i] > 0) ? max[i] : min[i];
    return res + getCenter();
}

Physics::Collider::Collider() : mass(0), localCentroid(0), localInertiaTensor(0) {
	Engine::addCollider(this);
}
