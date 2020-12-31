#include "ImpulseBased.h"
#include "../../Componets/RigidBody.h"
#include <glm.hpp>
#include <gtx/string_cast.hpp>
#include <vector>

#define E 1.0f
void Physics::ImpulseBased::resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold)
{
	const std::vector<Component::RigidBody*> rbs = { a, b };
    // const float j = getImpuseForce(b, a, manafold);
    glm::vec3 im(0);
    const glm::vec3 n = glm::normalize(manafold.normal);
    for (int i = 0; i < manafold.points.size(); i++) {
        const glm::vec3& point = manafold.points[i];

        const glm::vec3 ra = point - a->getPosition();
        const glm::vec3 rb = point - b->getPosition();

        const glm::vec3 rv = b->getVelocity() + glm::cross(b->getAngularVelocity(), rb) -
            a->getVelocity() - glm::cross(a->getAngularVelocity(), ra);

        const float contactVel = glm::dot(rv, n);

        /*if (contactVel > 0) // moving away
            break;*/

        const glm::vec3 raCrossN = glm::cross(ra, n);
        const glm::vec3 rbCrossN = glm::cross(rb, n);

        // float invMassSum = a->getInverseMass() + b->getInverseMass() + glm::dot(glm::sqrt(raCrossN) * a->getGlobalInverseInertiaTensor() + glm::sqrt(rbCrossN) * b->getGlobalInverseInertiaTensor(), n);
        float invMassSum = a->getInverseMass() + b->getInverseMass() + glm::dot(glm::cross((raCrossN), ra) * a->getGlobalInverseInertiaTensor() + glm::cross((rbCrossN), rb) * b->getGlobalInverseInertiaTensor(), n);

        float j = -(1.0f + E) * contactVel;

        j /= invMassSum;
        j /= static_cast<float>(manafold.points.size());


        j = -(1 + E) * glm::dot(b->linearVelocity - a->linearVelocity, n);
        auto t = glm::dot(n, n * (a->getInverseMass() + b->getInverseMass()));
        j /= t;
        j /= static_cast<float>(manafold.points.size());
        const glm::vec3 impulse = j * n;

        // a->applyAccAt(-impulse, point);
        // b->applyAccAt(impulse, point);
        //b->getAngularVelocity() += glm::cross(rb, impulse);

        // b->getVelocity() = impulse * b->getInverseMass();
        im += impulse;
        // std::cout << glm::to_string(b->getPosition()) << std::endl;
    }
    b->getVelocity() += im * b->getInverseMass();
    // positionalCorrection(a, b, manafold);
    // std::cout << glm::to_string(impulse) << "#" << std::endl;

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

void Physics::ImpulseBased::positionalCorrection(Component::RigidBody* a, Component::RigidBody* b, const Physics::CollisionManfold& info)
{
    const float k_slop = 0.05f; // Penetration allowance
    const float percent = 0.4f; // Penetration percentage to correct
    glm::vec3 correction = (fmaxf(info.penetration - k_slop, 0.0f) / (a->getInverseMass() + b->getInverseMass())) * info.normal * percent;
    a->getPosition() -= correction * a->getInverseMass();
    b->getPosition() += correction * b->getInverseMass();
}
