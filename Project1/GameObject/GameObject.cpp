#include "GameObject.h"

GameObject::GameObject() : componets(), enabled(), transform(new Component::Transform()) {
	// this->addComponet(new Component::Transform());
	enabled.push_back(1);
}

void GameObject::tick(short currentTick)
{
	for (unsigned i = 0; i < componets.size(); i++) {
		if (!enabled[i]) continue;
		Component::Base*& comp = componets[i];
		comp->update();
		if (currentTick == FIXED_UPDATE_RATE) {
			comp->fixedUpdate();
		}
	}
}

void GameObject::cleanUp()
{
	for (Component::Base*& componet : componets) {
		componet->cleanUp();
		delete componet;
		componet = nullptr;
	}
	componets.clear();
	transform->cleanUp();
	delete transform;
	transform = nullptr;
}
