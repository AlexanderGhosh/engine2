#pragma once
#include "Base.h"
#include "../CollisionDetection.h"
namespace Physics {
    class NSquared : public Broadphase {
    public:

        inline virtual void Add(AABB* aabb) { aabbs.push_back(aabb); };

        inline virtual void Update(void) { };

        virtual const ColliderPairList& ComputePairs();
        virtual Collider* Pick(const glm::vec3& point) const;
        virtual void Query(const AABB& aabb, ColliderList& out) const;
        virtual RayCastResult RayCast(const Ray3& ray) const;

    private:

        std::vector<AABB*> aabbs;
        ColliderPairList pairs;
    };
};
