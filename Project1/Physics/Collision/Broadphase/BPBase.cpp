#include "Base.h"
#include "../../Engine.h"

Physics::Broadphase::Broadphase() : colliders(&Engine::getColliders())
{
}
