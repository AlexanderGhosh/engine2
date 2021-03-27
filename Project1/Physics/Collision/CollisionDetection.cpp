#include "CollisionDetection.h"
#include "Broadphase/Base.h"
#include "Narrowphase/Base.h"
#include <iostream>

Physics::Broadphase* Physics::CollisionDetection::broadphase = nullptr;
Physics::Narrowphase* Physics::CollisionDetection::narrowphase = nullptr;

const std::vector<Physics::CollisionManfold> Physics::CollisionDetection::getCollisions()
{
    if (!broadphase) {
        throw std::exception("No Broadphase given for collision detection");
        return { };
    }
	if (!narrowphase) {
		throw std::exception("No Narrowphase given for collision detection");
        return { };
	}
    std::vector<CollisionManfold> res;
    const auto& pairs = broadphase->computePairs();
    for (const auto& pair : pairs) {
        if ((unsigned)pair[0] - (unsigned)pair[1] == 0)
            continue;
        CollisionManfold t = narrowphase->getCollisionData(pair[0], pair[1]);
        if (!t.collided)
            continue;
        res.push_back(t);
    }
    return res;
}

bool Physics::CollisionDetection::checkCollision(const AABB* a, const AABB* b)
{
    return glm::all(glm::lessThanEqual(a->getMin(), b->getMax()) && glm::greaterThanEqual(a->getMax(), b->getMin()));
}

bool Physics::CollisionDetection::checkCollision(const AABB* a, const glm::vec3 b)
{
    return glm::all(glm::lessThanEqual(b, a->getMax()) && glm::greaterThanEqual(b, a->getMin()));
}

Physics::CollisionManfold Physics::CollisionDetection::getCollisionData(AABB* a, AABB* b)
{
    CollisionManfold res;
    res.collided = true;
    res.bodies[0] = a;
    res.bodies[1] = b;
    res.penetration = 0;
    res.normal = glm::normalize(*a->position - *b->position);
    res.points[0] = *a->position + res.normal * a->getMaxRaw() * *a->scale;
    res.points[0] = *b->position + res.normal * b->getMinRaw() * *b->scale;
    return res; 
}

void Physics::CollisionDetection::cleanUp()
{
    if (broadphase) {
        broadphase->cleanUp();
        // delete broadphase;
        broadphase = nullptr;
    }

    if (narrowphase) {
        narrowphase->cleanUp();
        // delete narrowphase;
        narrowphase = nullptr;
    }
}

float Physics::CollisionManfold::getRestitution() const
{
    return 0.5f;
}
