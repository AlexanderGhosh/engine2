#pragma once
#include "Collider.h"
#include <unordered_map>
#include <list>
#include <glm.hpp>
#include "../../Utils/General.h"


namespace Physics {
    class Broadphase;
    struct CollisionManfold {
        bool collided;
        glm::vec3 normal;
        glm::vec3 pointsOfCollision[2];
        float minmumTranslation;
        Collider* bodies[2];
        CollisionManfold() : collided(0), normal(0), pointsOfCollision{ Utils::zero(), Utils::zero() }, minmumTranslation(0), bodies{ nullptr, nullptr } { };
        inline void cleanUp() {
            for (char i = 0; i < 2; i++) {
                delete &pointsOfCollision[i];
                bodies[i] = nullptr;
            }
            delete pointsOfCollision;
            delete bodies;
            *bodies = nullptr;
        };
        inline ~CollisionManfold() { cleanUp(); };
    };
    class CollisionDetection {
    private:
        static std::vector<Collider*> allColliders;
        static Broadphase* broadphase;
    public:
        static const std::vector<CollisionManfold>& getCollisions();
        static inline void setBroadphase(Broadphase* algo) { broadphase = algo; };
        static bool checkCollision(const AABB* a, const AABB* b);
        static bool checkCollision(const AABB* a, const glm::vec3 b);
        static CollisionManfold getCollisionData(AABB* a, AABB* b);
        inline static void cleanUp() {
            auto it = allColliders.end();
            for (auto it = allColliders.end(); it != allColliders.begin();) {
                auto collider = *it;
                collider->cleanUp();
                delete collider;
                allColliders.erase(it);
            }
        };
    };
};

