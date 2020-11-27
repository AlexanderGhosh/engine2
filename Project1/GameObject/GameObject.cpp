#include "GameObject.h"

GameObject::GameObject() : componets(), enabled(), transform(new Component::Transform()) {
	// this->addComponet(new Component::Transform());
	enabled.push_back(1);
}

void GameObject::addComponet(Component::Base* componet)
{
	componets.push_back(componet);
	enabled.push_back(1);
	componet->setParent(this);
	if (componet->getType() == Component::Type::Collider) {
		Component::RigidBody* rb = getRigidbody();
		if (!rb)
			return;
		rb->addCollider(reinterpret_cast<Physics::Collider*>(componet));
	}
	else if (componet->getType() == Component::Type::Rigidbody) {
		Component::RigidBody* rb = reinterpret_cast<Component::RigidBody*>(componet);
		for (Component::Base* comp : componets) {
			if (comp->getType() == Component::Type::Collider) {
				rb->addCollider(reinterpret_cast<Physics::Collider*>(comp));
			}
		}
	}
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

Component::RigidBody* GameObject::getRigidbody()
{
	for (Component::Base* comp : componets) {
		if (comp->getType() == Component::Type::Rigidbody)
			return reinterpret_cast<Component::RigidBody*>(comp);
	}
	return nullptr;
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
