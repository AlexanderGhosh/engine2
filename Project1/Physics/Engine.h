#pragma once
#include "../Componets/Rigidbody.h"
#include "Collision/CollisionDetection.h"
#include "../Gizmos/GizmoShapes.h"

#define E 1.0f
#define COEF_REST 1.0f
namespace Physics {
	class Resolution;
	class Engine
	{
	private:
		static std::vector<Component::RigidBody*> rigidbodies;
		static std::vector<Collider*> colliders;
		static Resolution* resolution;
		static glm::vec3 gravity;
		static float dampping, deltaTime;
		static std::list<Gizmos::Point> tempGizmos;
		static Gizmos::Line* tempNormal;

		static void drawManafold(const Physics::CollisionManfold& manafold);
	public:
		static int roundResolution;
		static void update();
		static void cleanUp();
		static inline void addCollider(Collider* collider) {
			colliders.push_back(collider);
		};
		static inline void addRigidBody(Component::RigidBody* rigidbody) {
			rigidbodies.push_back(rigidbody);
		};
		template<class T>
		static inline void setResponse() { resolution = DBG_NEW T(); };
		static inline std::vector<Collider*>& getColliders() { return colliders; };

		static inline glm::vec3& getGravity() { return gravity; };
		static inline float& getDamppingFactor() { return dampping; };
		static inline float getDeltaTime() { return deltaTime; };
		static inline void setDeltaTime(Float dt) { deltaTime = dt; };
	};
};

