#include "NSquared.h"
#include <algorithm>
const Physics::ColliderPairList& Physics::NSquared::ComputePairs()
{
    pairs.clear();

    // outer loop
    auto end = aabbs.end();
    for (auto i = aabbs.begin(); i != end; ++i)
    {

        // inner loop
        auto jStart = i;
        for (auto j = ++jStart; j != end; ++j)
        {
            AABB* aabbA = *i;
            AABB* aabbB = *j;
            Collider* colliderA = aabbA->getCollider();
            Collider* colliderB = aabbB->getCollider();
            /*RigidBody* bodyA = colliderA->Body();
            RigidBody* bodyB = colliderB->Body();

            // skip same-body collision
            if (bodyA == bodyB)
                continue;*/

                // add collider pair
            if (CollisionDetection::checkCollision(aabbA, aabbB))
                pairs.emplace_back(colliderA, colliderB);

        }
    }

    return pairs;
}

Physics::Collider* Physics::NSquared::Pick(const glm::vec3& point) const
{
    for (const auto& aabb : aabbs)
        if (CollisionDetection::checkCollision(aabb, point))
            return reinterpret_cast<Collider*>(aabb);
    return nullptr;
}

void Physics::NSquared::Query(const AABB& aabb, ColliderList& out) const
{
    for (AABB* colliderAABB : aabbs)
        if (CollisionDetection::checkCollision(colliderAABB, &aabb))
            out.push_back(colliderAABB->getCollider());
}

Physics::RayCastResult Physics::NSquared::RayCast(const Ray3& ray) const
{
    return { };

    // test AABBs for candidates
    std::vector<Collider*> candidateList;
    candidateList.reserve(aabbs.size());
    for (AABB* aabb : aabbs)
        if (false) // aabb->TestRay(ray)
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

    return result;
}

