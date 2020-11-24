#include "NSquared.h"
#include <algorithm>
const Physics::ColliderPairList& Physics::NSquared::computePairs()
{
    pairs.clear();

    auto end = colliders->end();
    for (auto i = colliders->begin(); i != end; ++i)
    {
        auto jStart = i;
        for (auto j = ++jStart; j != end; ++j)
        {
            const AABB* aabbA = (*i)->constructAABB();
            const AABB* aabbB = (*j)->constructAABB();
            // Collider* colliderA = aabbA->getCollider();
            // Collider* colliderB = aabbB->getCollider();
            /*RigidBody* bodyA = colliderA->Body();
            RigidBody* bodyB = colliderB->Body();

            // skip same-body collision
            if (bodyA == bodyB)
                continue;*/

                // add collider pair
            if (CollisionDetection::checkCollision(aabbA, aabbB))
                pairs.emplace_back(*i, *j);

        }
    }

    return pairs;
}

Physics::Collider* Physics::NSquared::pick(const glm::vec3& point) const
{
    for (auto& collider : *colliders)
        if (CollisionDetection::checkCollision(collider->constructAABB(), point))
            return reinterpret_cast<Collider*>(collider);
    return nullptr;
}

void Physics::NSquared::query(const AABB& aabb, ColliderList& out) const
{
    for (auto& collider : *colliders)
        if (CollisionDetection::checkCollision(collider->constructAABB(), &aabb))
            out.push_back(collider->getCollider());
}

Physics::RayCastResult Physics::NSquared::rayCast(const Ray3& ray) const
{
    return { };
    /*
    // test AABBs for candidates
    std::vector<Collider*> candidateList;
    candidateList.reserve(colliders->size());
    for (AABB* aabb : aabbs)
        if (false) // collider->TestRay(ray)
            candidateList.push_back(aabb->getCollider());

    // struct for storing ray-collider test results
    struct ResultEntry
    {
        Collider* collider;
        float t;
        glm::vec3 normal;

        bool operator<(const ResultEntry& rhs) const
        {
            // smaller t = closer
            return t > rhs.t;
        }
    };
    typedef std::vector<ResultEntry> ResultList;

    // test actual colliders
    ResultList resultList;
    resultList.reserve(candidateList.size());
    for (Collider* collider : candidateList)
    {
        // hit point = ray.pos + t * ray.dir
        float t;
        glm::vec3 normal;
        if (false) // collider->TestRay(ray, t, n)
        {
            ResultEntry entry = { collider, t, normal };
            resultList.push_back(entry);
        }
    }

    // sort the result list
    std::sort(resultList.begin(), resultList.end());

    RayCastResult result;
    if (!resultList.empty())
    {
        // the first result entry is the closest one
        ResultEntry& entry = resultList.front();
        result.hit = true;
        result.collider = entry.collider;
        result.t = entry.t;
        result.normal = entry.normal;
        result.intersection = ray.pos + entry.t * ray.dir;
    }
    else
        result.hit = false;

    return result;*/
}