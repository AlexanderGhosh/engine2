#pragma once
#include <gtx/quaternion.hpp>
#include "../Utils/General.h"


class GameObject;
namespace Component {
	enum class Type
	{
		Transform, Camera, AudioSource, Rigidbody, Collider, RenderMesh
	};
	class Base {
	public:
		virtual void update() { }; // every tick
		virtual void fixedUpdate() { }; // every x ticks
		virtual void cleanUp() = 0;
		virtual void setParent(GameObject* parent) { this->parent = parent; };
		inline GameObject* getParent() { return parent; };
		virtual inline Type getType() const = 0;
		virtual ~Base() = default;
	protected:
		inline Base() : parent(nullptr) { };
		GameObject* parent;
	};

	struct Transform : public Base {
		glm::vec3 Position, Scale;
		glm::quat Rotation;
		const glm::mat4 getModel() const;
		inline Transform(glm::vec3 p = Utils::zero(), glm::vec3 s = Utils::fill(1), glm::vec3 r = Utils::zero()) : Position(p), Scale(s), Rotation(r), Base() { };
		inline Type getType() const { return Type::Transform; };
		void cleanUp() { };
	};
}