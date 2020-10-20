#pragma once
#include "../Utils/General.h"

class GameObject;
namespace Componet {
	class Base {
	public:
		virtual void update() { }; // every tick
		virtual void fixedUpdate() { }; // every x ticks
		inline void setParent(GameObject* parent) { this->parent = parent; };
	protected:
		inline Base() : parent(nullptr) { };
		GameObject* parent;
	};

	struct Transform : public Base {
		glm::vec3 Position, Scale, Rotation;
		const glm::mat4 getModel() const;
		inline Transform(glm::vec3 p = { 0, 0, -5 }, glm::vec3 s = { 1, 1, 1 }, glm::vec3 r = { 0, 0, 0 }) : Position(p), Scale(s), Rotation(r), Base() { };
	};

	struct Ridgbody : public Base {
		glm::vec3 Velocity;
	};
}