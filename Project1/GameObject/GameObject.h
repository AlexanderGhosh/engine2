#pragma once
#define FIXED_UPDATE_RATE 20
#include <unordered_map>
#include "../Utils/General.h"

namespace Component {
	class ComponetBase;
	struct Transform;
	class RigidBody;
	class RenderMesh;
}
namespace UI {
	class Canvas;
}
enum class GameEventsTypes;
class GameScene;
class GameObject
{
protected:
	std::vector<Component::ComponetBase*> componets;
	Component::Transform* transform;
	std::vector<bool> enabled;
	bool alive;
	GameScene* scene;
	GameObject* parent;
	std::string name;
public:
	GameObject(String name = "");
	GameObject(Vector3 postion, Vector3 scale = glm::vec3(1));
	~GameObject() = default;
	void addComponet(Component::ComponetBase* componet);
	/// <summary>
	/// calls the componet's events eg update start
	/// </summary>
	void raiseEvents(const std::vector<GameEventsTypes>& events, Float deltaTime);
	void tick(short currentTick, float deltaTime);
	virtual void tryDraw(Float deltaTime);
	Component::Transform getGlobalTransform();
	Component::Transform* getLocalTransform();
	Component::RigidBody* getRigidbody();
	template<class T>
	inline T* getComponet()
	{
		for (Component::ComponetBase* componet : componets) {
			T* cast = dynamic_cast<T*>(componet);
			if (cast)
				return cast;
		}
		return nullptr;
	}
	void cleanUp();
	bool isAlive() const;
	void kill();
	void resurect();
	void setScene(GameScene* scene);
	GameScene* getScene();
	void processComponets();
	String getName() const;
	void setName(String name);
	void setParent(GameObject* parent);
	GameObject* getParent() const;
};

