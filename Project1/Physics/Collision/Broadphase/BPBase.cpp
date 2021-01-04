#include "Base.h"
#include "../../Engine.h"

void Physics::Broadphase::cleanUp()
{
	colliders->clear();
}

Physics::Broadphase::Broadphase(){
	colliders = &Engine::getColliders();
}


