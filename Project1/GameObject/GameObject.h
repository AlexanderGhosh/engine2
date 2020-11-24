#pragma once
#define FIXED_UPDATE_RATE 20
#include <vector>
#include "../Componets/Componets.h"
class GameObject
{
private:
	std::vector<Component::Base*> componets;
	Component::Transform* transform;
	std::vector<bool> enabled;
public:
	GameObject();
	inline void addComponet(Component::Base* componet) { 
		componets.push_back(componet); 
		enabled.push_back(1);
		componet->setParent(this);
	};
	void tick(short currentTick);
	inline Component::Transform* getTransform() const { return transform; };
	Component::Base* getRigidbody() { return nullptr; };
	void cleanUp();
};

