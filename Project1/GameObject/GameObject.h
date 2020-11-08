#pragma once
#define FIXED_UPDATE_RATE 20
#include <vector>
#include "../Componets/Componets.h"
class GameObject
{
private:
	std::vector<Componet::Base*> componets;
	Componet::Transform* transform;
	std::vector<bool> enabled;
public:
	GameObject();
	inline void addComponet(Componet::Base* componet) { 
		componets.push_back(componet); 
		enabled.push_back(1);
		componet->setParent(this);
	};
	void tick(short currentTick);
	inline Componet::Transform* getTransform() const { return transform; };
	Componet::Base* getRigidbody() { return nullptr; };
	void cleanUp();
};

