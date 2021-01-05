#pragma once
#define FIXED_UPDATE_RATE 20
#include <vector>
#include "../Componets/Componets.h"
#include "../Componets/RigidBody.h"
class GameObject
{
private:
	std::vector<Component::ComponetBase*> componets;
	Component::Transform* transform;
	std::vector<bool> enabled;
public:
	GameObject();
	void addComponet(Component::ComponetBase* componet);
	void tick(short currentTick);
	void tryDraw();
	inline Component::Transform* getTransform() const { return transform; };
	Component::RigidBody* getRigidbody();
	template<class T>
	T* getComponet();
	void cleanUp();
};

