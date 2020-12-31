#pragma once
#include "Collider.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <glm.hpp>
#include "../../Utils/General.h"


namespace Physics {
    class Broadphase;
    class Narrowphase;
    struct CollisionManfold {
        bool collided;
        glm::vec3 normal;
        std::vector<glm::vec3> points;
        float penetration;
        std::array<Collider*, 2> bodies;
        CollisionManfold() : collided(0), normal(0), points{ Utils::zero(), Utils::zero() }, penetration(0), bodies{ nullptr, nullptr } { };
        inline void cleanUp() {
            for (char i = 0; i < 2; i++) {
                bodies[i] = nullptr;
            }
        };
        float getRestitution() const;
        // inline ~CollisionManfold() { cleanUp(); };
    };
    class CollisionDetection {
    private:
        static Broadphase* broadphase;
        static Narrowphase* narrowphase;
    public:
        static const std::vector<CollisionManfold> getCollisions();
        template<class T>
        static inline void setBroadphase() { broadphase = new T(); };
        template<class T>
        static inline void setNarrowphase() { narrowphase = new T(); };
        static bool checkCollision(const AABB* a, const AABB* b);
        static bool checkCollision(const AABB* a, const glm::vec3 b);
        static CollisionManfold getCollisionData(AABB* a, AABB* b);
        static void cleanUp();
    };
};

