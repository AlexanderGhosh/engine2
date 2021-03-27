#include "NSquared.h"
#include "../../../Componets/RigidBody.h"
#include <algorithm>
Physics::NSquared::NSquared() : Broadphase()
{
    pairs = std::vector<std::array<Physics::Collider*, 2>>();
}
const ColliderPairList& Physics::NSquared::computePairs()
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

            // skip same-body collision
            if (aabbA == aabbB)
                continue;
                // add collider pair
            if (CollisionDetection::checkCollision(aabbA, aabbB))
                pairs.push_back({ *i, *j });

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

void Physics::NSquared::cleanUp()
{
    Broadphase::cleanUp();
    for (auto itt = pairs.begin(); itt != pairs.end();) {
        for (char i = 0; i < 2; i++) {
            (*itt)[i]->cleanUp();
        }
        delete[2] (*itt).data();
        itt = pairs.erase(itt);
    }
    auto t = sizeof(pairs);
    pairs.clear();
}
