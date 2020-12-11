#pragma once
#define FIXED_UPDATE_RATE 20
#include <vector>
#include "../Componets/Componets.h"
#include "../Componets/RigidBody.h"
class GameObject
{
private:
	std::vector<Component::Base*> componets;
	Component::Transform* transform;
	std::vector<bool> enabled;
public:
	GameObject();
	void addComponet(Component::Base* componet);
	void tick(short currentTick);
	inline Component::Transform* getTransform() const { return transform; };
	Component::RigidBody* getRigidbody();
	template<class T>
	T* getComponet();
	void cleanUp();
};

template<class T>
T* GameObject::getComponet()
{
	for (Component::Base* componet : componets) {
		T* cast = dynamic_cast<T*>(componet);
		if (cast)
			return cast;
	}
}
