#pragma once
#include <gtx/quaternion.hpp>
#include "../Utils/General.h"


class GameObject;
namespace Component {
	enum class Type
	{
		Transform, Camera, AudioSource, Rigidbody, RenderMesh, Light, Animated, Script, Canvas, AudioReciever, ParticleEmmiter, CharacterController, ShadowCaster, Collider
	};
	class ComponetBase {
	public:
		// event callbacks
		virtual void awake() { };
		virtual void start() { };
		virtual void render(float deltaTime) { }; // every tick (may change later)
		virtual void update(float deltaTime) { }; // every tick
		virtual void fixedUpdate(float deltaTime) { }; // every x ticks
		virtual void mouseMove(float deltaTime) { };
		virtual void mouseButton(float deltaTime) { };
		virtual void keyButton(float deltaTime) { };

		virtual void cleanUp() = 0;
		virtual void setParent(GameObject* parent) { this->parent = parent; };
		inline GameObject* getParent() { return parent; };
		virtual inline Type getType() const = 0;
		virtual ~ComponetBase() = default;
	protected:
		inline ComponetBase() : parent(nullptr) { };
		GameObject* parent;
	};

	class Transform : public ComponetBase {
	public:
		glm::vec3 Position, Scale;
		glm::quat Rotation;
		const glm::mat4 getModel() const;
		Transform(glm::vec3 p = Utils::zero(), glm::vec3 s = Utils::fill(1), glm::vec3 r = Utils::zero());
		inline Type getType() const { return Type::Transform; };
		void rotate(Vector3 axis, Float angle);
		void rotate(Vector3 rotation);
		void cleanUp();

		Transform operator+(const Transform& a) const; 
		Transform& operator+=(const Transform& a);
		/*
		Num& operator+=(const Num& rhs) {

			this->m_iNumber += rhs.m_iNumber;
			return *this;
		}
		*/
	};
}