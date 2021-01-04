#pragma once
#include "Base.h"
#include "../CollisionDetection.h"
namespace Physics {
    class NSquared : public Broadphase {
    public:
        NSquared();
        inline virtual void add(AABB* aabb) { };

        inline virtual void update(void) { };

        virtual const ColliderPairList& computePairs();
        virtual Collider* pick(const glm::vec3& point) const;
        virtual void query(const AABB& aabb, ColliderList& out) const;
        virtual RayCastResult rayCast(const Ray3& ray) const;
        void cleanUp();
        ~NSquared() = default;
    private:
        ColliderPairList pairs;
    };
};
