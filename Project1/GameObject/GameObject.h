#pragma once
#define FIXED_UPDATE_RATE 20
#include <vector>
#include "../Utils/General.h"

namespace Component {
	class ComponetBase;
	struct Transform;
	class RigidBody;
}
class GameObject
{
protected:
	std::vector<Component::ComponetBase*> componets;
	Component::Transform* transform;
	std::vector<bool> enabled;
public:
	GameObject();
	void addComponet(Component::ComponetBase* componet);
	void tick(short currentTick, float deltaTime);
	virtual void tryDraw(Float deltaTime);
	inline Component::Transform* getTransform() const { return transform; };
	Component::RigidBody* getRigidbody();
	template<class T>
	T* getComponet();
	void cleanUp();
};

