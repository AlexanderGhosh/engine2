#include "ImpulseBased.h"
#include "../../Componets/RigidBody.h"
#include <glm.hpp>
#include <vector>

void Physics::ImpulseBased::resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold)
{
	const std::vector<Component::RigidBody*> rbs = { a, b };
    // const float j = getImpuseForce(b, a, manafold);

    const glm::vec3 n = glm::normalize(manafold.normal);

    for (int i = 0; i < 2; i++) {
        const glm::vec3& point = manafold.points[i];

        const glm::vec3 ra = point - a->getPosition();
        const glm::vec3 rb = point - b->getPosition();

        const glm::vec3 rv = b->getVelocity() + glm::cross(b->getAngularVelocity(), rb) -
            a->getVelocity() - glm::cross(a->getAngularVelocity(), ra);

        const float contactVel = glm::dot(rv, n);

        if (contactVel > 0)
            return;

        glm::vec3 raCrossN = glm::cross(ra, n);
        glm::vec3 rbCrossN = glm::cross(rb, n);

        float invMassSum = a->getInverseMass() + b->getInverseMass() + glm::dot(glm::sqrt(raCrossN) * a->getGlobalInverseInertiaTensor() + glm::sqrt(rbCrossN) * b->getGlobalInverseInertiaTensor(), n);
        invMassSum = a->getInverseMass() + b->getInverseMass() + glm::dot(glm::cross((raCrossN), ra) * a->getGlobalInverseInertiaTensor() + glm::cross((rbCrossN), rb) * b->getGlobalInverseInertiaTensor(), n);

        float j = -(1.0f + 1) * contactVel;

        j /= invMassSum;
        j /= 2.0f;

        const glm::vec3 impulse = j * manafold.normal;

        a->applyAccAt(-impulse, point);
        b->applyAccAt( impulse, point);
    }

    /*
	char i = 0;
	for (Component::RigidBody* rb : rbs) {
		// const glm::vec3 f = getCoulombFriction(info, j);


        float d = glm::dot(rb->linearVelocity * rb->getMass(), manafold.normal);

        float j = glm::max(-(1 + 1) * d, 0.0f);

        // rb->linearVelocity += (j * manafold.normal * (i ? -1.0f : 1.0f)) / rb->getMass();
		glm::vec3 deltaV = (j * manafold.normal * (i ? 1.0f : -1.0f)) / rb->getMass();
		rb->getVelocity() += deltaV;

		glm::vec3 r = manafold.points[i++] - rb->getPosition();
        auto T = glm::cross(r, j * manafold.normal) * rb->getGlobalInverseInertiaTensor();
        rb->getAngularVelocity() += -T;


		// float d = glm::dot(rb->getVelocity() * rb->getMass(), info.normal);
		// float j1 = glm::max(-(1.0f + 1) * d, 0.0f);
		// glm::vec3 deltaV1 = (j1 * info.normal) / rb->getMass();
	}*/
}
float Physics::ImpulseBased::getImpuseForce(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& info)
{
    const float e = 1; // a->material.coefficientOfRestitution* b->material.coefficientOfRestitution;

    const float mA = 1.0f / a->getMass();
    const float mB = 1.0f / b->getMass();

    const glm::vec3& WA = a->getAngularVelocity();
    const glm::vec3& WB = b->getAngularVelocity();

    const glm::vec3& xA = a->getPosition();
    const glm::vec3& xB = b->getPosition();

    const glm::vec3& VA = a->getVelocity();
    const glm::vec3& VB = b->getVelocity();

    const glm::mat3 IA = a->getGlobalInverseInertiaTensor();
    const glm::mat3 IB = b->getGlobalInverseInertiaTensor();

    const glm::vec3& n = info.normal;

    const glm::vec3& pA = info.points[0];
    const glm::vec3& pB = info.points[1];

    // Calculated
    const glm::vec3 rA = pA - xA;
    const glm::vec3 rB = pB - xB;

    const glm::vec3 PdotA = a->isKinimatic ? glm::vec3(0) : VA + glm::cross(WA, rA);
    const glm::vec3 PdotB = b->isKinimatic ? glm::vec3(0) : VB + glm::cross(WB, rB);
    const float Vrel = glm::dot(n, PdotB - PdotA);

    // neumerator
    const float neumerator = -(1.0f + e) * Vrel;

    // denominator
    glm::vec3 intemediate = glm::cross(glm::cross(rA, n), rA); // dynamic object
    intemediate = IA * intemediate;
    if (!b->isKinimatic || true) { // static object
        intemediate += IB * glm::cross(glm::cross(rB, n), rB);
    }
    float denominator = glm::dot(intemediate, n);
    denominator += mA;
    if (!b->isKinimatic || true) { // static object
        denominator += mB;
    }

    float j = neumerator / denominator;
    return j;
}
