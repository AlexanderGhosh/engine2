#include "Engine.h"
#include <glm.hpp>
#include "Collision/Collider.h"
#include "../GameObject/GameObject.h"
#include "../Componets/RigidBody.h"
#include "Collision/Colliders/SATBaseCollider.h"
#include "DebugRenderer.h"

std::list<Component::Rigidbody*> Physics::Engine::rigidbodies = {};
std::list<Physics::Collider*> Physics::Engine::colliders = {};
glm::vec3 Physics::Engine::gravity = { 0, -1, 0 };
Physics::Narrowphase* Physics::Engine::narrowphase = nullptr;
bool Physics::Engine::RenderDebug = true;

std::vector<Physics::CollisionDetails> Physics::Engine::getIntersections()
{
	const float epsilon = 0.001f;
	std::vector<Physics::CollisionDetails> details;
	for (auto i = colliders.begin(); i != colliders.end(); i++) {
		for (auto j = std::next(i); j != colliders.end(); j++) {
			Physics::Collider* a = *i;
			Physics::Collider* b = *j;

			const float radius_sum = a->getRadius(45) + b->getRadius(45);

			const glm::vec3 a_pos = a->getAbsolutePosition();
			const glm::vec3 b_pos = b->getAbsolutePosition();

			const float dist_ = glm::distance(a_pos, b_pos);
			float intersection_distance = radius_sum - dist_;

			if (intersection_distance >= epsilon) {
				int t = 0;
				// intersection detected
				/*Physics::CollisionDetails data {};

				data.a = a;
				data.b = b;

				const glm::vec3 relative_hit_a = glm::normalize(b_pos - a_pos) * a->getRadius(45);
				const glm::vec3 relative_hit_b = glm::normalize(a_pos - b_pos) * b->getRadius(45);

				data.a_hit = a_pos + relative_hit_a;
				data.b_hit = b_pos + relative_hit_b;

				data.intersection_norm = glm::normalize(b_pos - a_pos);
				data.intersection_distance = intersection_distance;

				data.coef_restitution = 1;

				details.push_back(data);*/
				}

			Physics::CollisionManfold hit = narrowphase->getCollisionData(a, b);
			if (hit.hit) {
				// intersection detected
				Physics::CollisionDetails data {};

				data.a = a;
				data.b = b;

				const glm::vec3 relative_hit_a = glm::normalize(b_pos - a_pos) * a->getRadius(45);
				const glm::vec3 relative_hit_b = glm::normalize(a_pos - b_pos) * b->getRadius(45);
				
				data.a_hit = a_pos + relative_hit_a;
				data.b_hit = b_pos + relative_hit_b;

				data.intersection_norm = glm::normalize(b_pos - a_pos);
				data.intersection_distance = intersection_distance;

				// data.intersection_norm = hit.normal;
				// data.intersection_distance = hit.depth;
				std::cout << glm::to_string(glm::vec4(hit.normal, hit.depth)) << std::endl;

				data.coef_restitution = 1;

				details.push_back(data);

			}
		}
	}
	return details;
}

void Physics::Engine::update()
{
	if (RenderDebug) {
		for (auto& col : colliders) {
			DebugRenderer::addCollider(col);
		}
	}
	for (auto rb : rigidbodies) {
		rb->applyForce(gravity);
	}

	auto intersections = getIntersections();
	resolveIntersections(intersections);
	int i = 0;

	DebugRenderer::update();
}

void Physics::Engine::resolveIntersections(const std::vector<Physics::CollisionDetails>& details)
{
	for (const auto& intercetion : details) {
		GameObject* a = intercetion.a->getParent();
		GameObject* b = intercetion.b->getParent();

		Component::Rigidbody* a_rb = a->getComponet<Component::Rigidbody>();
		Component::Rigidbody* b_rb = b->getComponet<Component::Rigidbody>();

		a_rb->velocity *= 0;
		b_rb->velocity *= 0;

		/*a_rb->backPeddle(intercetion.intersection_distance);
		b_rb->backPeddle(intercetion.intersection_distance);

		const glm::vec3 impulse = calcImpulseForce(a_rb, b_rb, intercetion.intersection_norm, intercetion.coef_restitution);

		a_rb->applyForce(-impulse);
		b_rb->applyForce(impulse);*/
	}
}

void Physics::Engine::cleanUp()
{
	if (RenderDebug) {
		DebugRenderer::cleanUp();
	}
}

void Physics::Engine::addRigidbody(Component::Rigidbody* rb)
{
	rigidbodies.push_back(rb);
}

void Physics::Engine::addCollider(Physics::Collider* col)
{
	colliders.push_back(col);
}

// b hits a therefore the norm is from a to b (points away from a)
glm::vec3 Physics::Engine::calcImpulseForce(Component::Rigidbody* a, Component::Rigidbody* b, Vector3 norm, Float coef_restitution) {
	const glm::vec3 r_a = norm;
	const glm::vec3 r_b = -norm;

	const glm::vec3 v_pa = a->velocity + glm::cross(a->angularVelocity, r_a);
	const glm::vec3 v_pb = b->velocity + glm::cross(b->angularVelocity, r_b);
	Float e = coef_restitution;

	const auto v_r = v_pb - v_pa;

	const float numerator = -(1.f + e) * glm::dot(v_r, norm);
	float denominator = a->getInvMass() + b->getInvMass();
	const glm::vec3 angular_component_a = a->getGlobalInvInertiaTensor() * glm::cross(glm::cross(r_a, norm), r_a);
	const glm::vec3 angular_component_b = b->getGlobalInvInertiaTensor() * glm::cross(glm::cross(r_b, norm), r_b);

	denominator += glm::dot(angular_component_a + angular_component_b, norm);

	const float jr = numerator / denominator;


	return jr * norm;
}
