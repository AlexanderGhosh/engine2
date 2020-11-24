#pragma once
#include "../Collider.h"
#include <glm.hpp>
#include <unordered_map>
#include <list>
namespace Physics {
    typedef std::pair<Collider*, Collider*> ColliderPair;
    typedef std::list<ColliderPair> ColliderPairList;
    typedef std::vector<Collider*> ColliderList;
    struct Ray3 {
        glm::vec3 pos, dir;
        inline Ray3() : pos(0), dir(0) { };
    };
    struct RayCastResult {
        bool hit;
        Collider* collider;
        glm::vec3 normal, intersection;
        float t;
        inline RayCastResult() :
            hit(0), collider(nullptr), normal(0), intersection(0), t(0) { };
    };
    class Broadphase {
    public:
        // adds a new AABB to the broadphase
        virtual void add(AABB* aabb) = 0;

        // updates broadphase to react to changes to AABB
        virtual void update(void) = 0;

        // returns a list of possibly colliding colliders
        virtual const ColliderPairList& computePairs(void) = 0;

        // returns a collider that collides with a point
        // returns null if no such collider exists
        virtual Collider* pick(const glm::vec3& point) const = 0;

        // returns a list of colliders whose AABBs collide 
        // with a query AABB
        virtual void query(const AABB& aabb, ColliderList& output) const = 0;

        // result contains the first collider the ray hits
        // result contains null if no collider is hit
        virtual RayCastResult rayCast(const Ray3& ray) const = 0;

        virtual void cleanUp() { };
        std::vector<Collider*>* colliders;
    protected:
        Broadphase();
    };
};