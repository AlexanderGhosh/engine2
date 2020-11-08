#pragma once
#include "../Utils/General.h"

class GameObject;
namespace Componet {
	class Base {
	public:
		virtual void update() { }; // every tick
		virtual void fixedUpdate() { }; // every x ticks
		virtual void cleanUp() { parent = nullptr; };
		inline void setParent(GameObject* parent) { this->parent = parent; };
		inline const GameObject* getParent(GameObject* parent) const { return parent; };
	protected:
		inline Base() : parent(nullptr) { };
		GameObject* parent;
	};

	struct Transform : public Base {
		glm::vec3 Position, Scale, Rotation;
		const glm::mat4 getModel() const;
		inline Transform(glm::vec3 p = Utils::zero(), glm::vec3 s = Utils::fill(1), glm::vec3 r = Utils::zero()) : Position(p), Scale(s), Rotation(r), Base() { };
	};
}