#include "ImpulseBased.h"
#include "../../Componets/RigidBody.h"
#include "../Collision/CollisionDetection.h"
#include <glm.hpp>
#include <gtx/string_cast.hpp>
#include <vector>

#define E 0.0f
void Physics::ImpulseBased::resolve(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold)
{
    const glm::vec3 n = glm::normalize(manafold.normal);
    float im = 0;
    glm::vec3 ang1(0), ang2(0);
    for (int i = 0; i < manafold.points.size(); i++) {
        Vector3 point = manafold.points[i];

        float numerator = 0;
        float denominator = 0;

        glm::vec3 r1 = point - a->getCOS();
        glm::vec3 r2 = point - b->getCOS();

        glm::vec3 vp1 = a->getVelocity() + glm::cross(a->getAngularVelocity(), r1);
        glm::vec3 vp2 = b->getVelocity() + glm::cross(b->getAngularVelocity(), r2);

        const glm::vec3 vr = vp1 - (!b->isKinimatic ? vp2 : glm::vec3(0));

        numerator = -(1.0f + E) * glm::dot(vr,  n);

        denominator += a->getInvMass();
        if (!b->isKinimatic) {
            denominator += b->getInvMass();
        }
        glm::vec3 temp = glm::cross(a->getInvInertia_G() * glm::cross(r1, n), r1);
        if (!b->isKinimatic) {
            temp += glm::cross(b->getInvInertia_G() * glm::cross(r2, n), r2);
        }
        denominator += glm::dot(temp, n);


        /*glm::vec3 vab = a->getVelocity() - b->getVelocity();

        numerator = -(1 + E) * glm::dot(vab, n);

        denominator = a->getInvMass();
        denominator += b->getInvMass();
        //denominator = glm::dot(n, n * denominator);
        */
        assert(denominator > 0);
        float j = numerator / denominator;
        im += j;

        ang1 += glm::cross(r1, j * n);
        ang2 += glm::cross(r2, j * n);
    }
    im /= static_cast<float>(manafold.points.size() ? manafold.points.size() : 1);
    // im *= 0.5f;
    std::cout << std::to_string(im / 10.0f) << std::endl;
    a->velocityAdder(n * im * a->getInvMass());
    b->velocityAdder(n * im * -b->getInvMass());

    a->angularVelAdder(a->getInvInertia_G() * ang1);
    b->angularVelAdder(b->getInvInertia_G() * -ang2);
    std::cout << "---------------------------------------------------------------\n";
}

float Physics::ImpulseBased::getBias(Component::RigidBody* a, Component::RigidBody* b, Physics::CollisionManfold& manafold)
{
    float bias = 0;
    const float b_scalar = 0.1f;
    const float b_slope = 0.001f;
    const float penetration_slop = fminf(manafold.penetration + b_slope, 0.0f);
    for (Vector3 point : manafold.points) {
        const glm::vec3 ra = point - a->getCOS();
        const glm::vec3 rb = point - b->getCOS();
        bias += -(b_scalar * 60.0f) * penetration_slop;

        bias += (E * glm::dot(manafold.normal, a->getVelocity() + glm::cross(ra, a->getAngularVelocity()) - b->getVelocity() - glm::cross(rb, b->getAngularVelocity()))) / manafold.points.size();
    }

    return bias;
}

void Physics::ImpulseBased::positionalCorrection(Component::RigidBody* a, Component::RigidBody* b, const Physics::CollisionManfold& info)
{
    /*const float k_slop = 0.05f; // Penetration allowance
    const float percent = 0.4f; // Penetration percentage to correct
    glm::vec3 correction = (fmaxf(info.penetration - k_slop, 0.0f) / (a->getInverseMass() + b->getInverseMass())) * info.normal * percent;
    a->getPosition() -= correction * a->getInverseMass();
    b->getPosition() += correction * b->getInverseMass();*/
}
