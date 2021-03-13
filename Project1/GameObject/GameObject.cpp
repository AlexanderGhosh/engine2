#include "GameObject.h"
#include "../Componets/Componets.h"
#include "../Rendering/Rendering.h"
#include "../Componets/Animated.h"
#include "../Componets/RigidBody.h"
#include "../Scene/GameScene.h"

GameObject::GameObject(String name) : componets(), enabled(), transform(DBG_NEW Component::Transform()), alive(true), scene(nullptr), name(name) {
	enabled.push_back(1);
}

GameObject::GameObject(Vector3 postion, Vector3 scale) : GameObject()
{
	getTransform()->Position = postion;
	getTransform()->Scale = scale;
}

void GameObject::addComponet(Component::ComponetBase* componet)
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
		for (Component::ComponetBase* comp : componets) {
			if (comp->getType() == Component::Type::Collider) {
				rb->addCollider(reinterpret_cast<Physics::Collider*>(comp));
			}
		}
	}
	else if (componet->getType() == Component::Type::RenderMesh) {
		Component::Animated* comp = getComponet<Component::Animated>();
		if (comp) {
			reinterpret_cast<Component::RenderMesh*>(componet)->setAnimatedComp(comp);
		}
	}
	else if (componet->getType() == Component::Type::Animated) {
		Component::RenderMesh* comp = getComponet<Component::RenderMesh>();
		if (comp) {
			comp->setAnimatedComp(reinterpret_cast<Component::Animated*>(componet));
		}
	}
	if (scene)
		scene->processComponet(componet);
}

void GameObject::raiseEvents(const std::vector<GameEventsTypes>& events, Float deltaTime)
{
	for (int i = 0; i < componets.size(); i++) {
		Component::ComponetBase* comp = componets[i];
		for (const GameEventsTypes& type : events) {
			switch (type)
			{
			case GameEventsTypes::Update:
				comp->update(deltaTime);
				break;
			case GameEventsTypes::FixedUpdate:
				comp->fixedUpdate(deltaTime);
				break;
			case GameEventsTypes::MouseToggle:
				comp->mouseButton(deltaTime);
				break;
			case GameEventsTypes::MouseMove:
				comp->mouseMove(deltaTime);
				break;
			case GameEventsTypes::KeyToggle:
				comp->keyButton(deltaTime);
				break;
			case GameEventsTypes::Awake:
				comp->awake();
				break;
			case GameEventsTypes::Start:
				comp->start();
				break;
			}
		}
	}
}

void GameObject::tick(short currentTick, float deltaTime)
{
	for (unsigned i = 0; i < componets.size(); i++) {
		if (!enabled[i] OR componets[i]->getType() == Component::Type::RenderMesh OR componets[i]->getType() == Component::Type::Rigidbody) continue;
		Component::ComponetBase*& comp = componets[i];
		comp->update(deltaTime);
		if (currentTick == FIXED_UPDATE_RATE) {
			comp->fixedUpdate(deltaTime);
		}
	}
}

void GameObject::tryDraw(Float deltaTime)
{
	for (unsigned i = 0; i < componets.size(); i++) {
		if (enabled[i] AND componets[i]->getType() == Component::Type::RenderMesh) {
			Component::ComponetBase*& comp = componets[i];
			comp->update(deltaTime);
		}
	}
}

Component::RigidBody* GameObject::getRigidbody()
{
	for (Component::ComponetBase* comp : componets) {
		if (comp->getType() == Component::Type::Rigidbody)
			return reinterpret_cast<Component::RigidBody*>(comp);
	}
	return nullptr;
}

void GameObject::cleanUp()
{
	for (auto itt = componets.begin(); itt != componets.end();) {
		(*itt)->cleanUp();
		// delete* itt;
		itt = componets.erase(itt);
	}
	/*for (Component::ComponetBase*& componet : componets) {
		componet->cleanUp();
		delete componet;
		componet = nullptr;
	}*/
	componets.clear();
	transform->cleanUp();
	delete transform;
	transform = nullptr;
}

bool GameObject::isAlive() const
{
	return alive;
}

void GameObject::kill()
{
	alive = false;
}

void GameObject::resurect()
{
	alive = true;
}

void GameObject::setScene(GameScene* scene)
{
	this->scene = scene;
}

GameScene* GameObject::getScene()
{
	return scene;
}

void GameObject::processComponets()
{
	for(Component::ComponetBase* comp : componets)
		scene->processComponet(comp);
}

String GameObject::getName() const
{
	return name;
}

void GameObject::setName(String name)
{
	this->name = name;
}
