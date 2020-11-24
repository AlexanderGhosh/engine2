#pragma once
#include "Collider.h"
#include <unordered_map>
#include <list>
#include <glm.hpp>
#include "../../Utils/General.h"


namespace Physics {
    class Broadphase;
    class Narrowphase;
    struct CollisionManfold {
        bool collided;
        glm::vec3 normal;
        std::array<glm::vec3, 2> points;
        float penertraion;
        std::array<Collider*, 2> bodies;
        CollisionManfold() : collided(0), normal(0), points{ Utils::zero(), Utils::zero() }, penertraion(0), bodies{ nullptr, nullptr } { };
        inline void cleanUp() {
            for (char i = 0; i < 2; i++) {
                bodies[i] = nullptr;
            }
        };
        // inline ~CollisionManfold() { cleanUp(); };
    };
    class CollisionDetection {
    private:
        static Broadphase* broadphase;
        static Narrowphase* narrowphase;
    public:
        static const std::vector<CollisionManfold> getCollisions();
        static inline void setBroadphase(Broadphase* algo) { broadphase = algo; };
        static inline void setNarrowphase(Narrowphase* algo) { narrowphase = algo; };
        static bool checkCollision(const AABB* a, const AABB* b);
        static bool checkCollision(const AABB* a, const glm::vec3 b);
        static CollisionManfold getCollisionData(AABB* a, AABB* b);
        static void cleanUp();
    };
};

