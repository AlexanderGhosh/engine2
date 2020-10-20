#pragma once
#define FIXED_UPDATE_RATE 20
#include <vector>
#include "../Componets/Componets.h"
class GameObject
{
private:
	std::vector<Componet::Base*> componets;
	std::vector<bool> enabled;
public:
	GameObject();
	inline void addComponet(Componet::Base* componet) { 
		componets.push_back(componet); 
		enabled.push_back(1);
		componet->setParent(this);
	};
	void tick(short currentTick);
	inline Componet::Transform* getTransform() const { return dynamic_cast<Componet::Transform*>(componets[0]); };
};

