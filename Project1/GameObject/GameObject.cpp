#include "GameObject.h"

GameObject::GameObject() : componets(), enabled() {
	this->addComponet(new Componet::Transform());
}

void GameObject::tick(short currentTick)
{
	for (unsigned i = 0; i < componets.size(); i++) {
		if (!enabled[i]) continue;
		Componet::Base*& comp = componets[i];
		comp->update();
		if (currentTick == FIXED_UPDATE_RATE) {
			comp->fixedUpdate();
		}
	}
}

void GameObject::cleanUp()
{
	for (Componet::Base*& componet : componets) {
		componet->cleanUp();
		delete componet;
		componet = nullptr;
	}
}
