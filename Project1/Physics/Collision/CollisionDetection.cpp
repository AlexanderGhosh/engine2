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
    if (!pairs.empty()) {
        // std::cout << "passed broadphase\n";
    }
    for (const auto& pair : pairs) {
        auto t = narrowphase->getCollisionData(pair.first, pair.second);
        res.push_back(t);
    }
    if (res.size() > 0) {
        bool i = 0;
    }
    return res;
}

bool Physics::CollisionDetection::checkCollision(const AABB* a, const AABB* b)
{
    return glm::all(glm::lessThanEqual(a->min + a->getCenter(), b->max + b->getCenter()) && glm::greaterThanEqual(a->max + a->getCenter(), b->min + b->getCenter()));
}

bool Physics::CollisionDetection::checkCollision(const AABB* a, const glm::vec3 b)
{
    return glm::all(glm::lessThanEqual(b, a->max + a->getCenter()) && glm::greaterThanEqual(b, a->min + a->getCenter()));
}

Physics::CollisionManfold Physics::CollisionDetection::getCollisionData(AABB* a, AABB* b)
{
    CollisionManfold res;
    res.collided = true;
    res.bodies[0] = a;
    res.bodies[1] = b;
    res.penertraion = 0;
    res.normal = glm::normalize(a->center - b->center);
    res.points[0] = a->center + res.normal * a->max;
    res.points[0] = b->center + res.normal * b->min;
    return res;
}

void Physics::CollisionDetection::cleanUp()
{
    broadphase->cleanUp();
    delete broadphase;
    broadphase = nullptr;

    narrowphase->cleanUp();
    delete narrowphase;
    narrowphase = nullptr;
}