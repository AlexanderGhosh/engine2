#include "CollisionDetection.h"
#include "Broadphase/Base.h"


const std::vector<Physics::CollisionManfold>& Physics::CollisionDetection::getCollisions()
{
    std::vector<CollisionManfold> res;
    const auto& pairs = broadphase->ComputePairs();
    for (const auto& pair : pairs) {
        AABB* a = reinterpret_cast<AABB*>(pair.first);
        AABB* b = reinterpret_cast<AABB*>(pair.second);
        res.push_back(getCollisionData(a, b));
    }
    return res;
}

bool Physics::CollisionDetection::checkCollision(const AABB* a, const AABB* b)
{
    return glm::all(glm::lessThanEqual(a->min, b->max) && glm::greaterThanEqual(a->max, b->min));
}

bool Physics::CollisionDetection::checkCollision(const AABB* a, const glm::vec3 b)
{
    return glm::all(glm::lessThanEqual(b, a->max) && glm::greaterThanEqual(b, a->min));
}

Physics::CollisionManfold Physics::CollisionDetection::getCollisionData(AABB* a, AABB* b)
{
    CollisionManfold res;
    res.collided = true;
    res.bodies[0] = a;
    res.bodies[1] = b;
    res.minmumTranslation = 0;
    res.normal = glm::normalize(a->center - b->center);
    res.pointsOfCollision[0] = a->center + res.normal * a->max;
    res.pointsOfCollision[0] = b->center + res.normal * b->min;
    return res;
}
