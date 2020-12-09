#include "Base.h"
#include "..\Broadphase\Base.h"
#include "../../Engine.h"

Physics::SupportPoint Physics::Narrowphase:: support(const Collider* a, const Collider* b, const glm::vec3& dir)
{
    SupportPoint res;
    const glm::vec3 d = glm::normalize(dir);
    res.a = a->support(d);
    res.b = b->support(-d);
    res.v = res.a - res.b;
    return res;
}