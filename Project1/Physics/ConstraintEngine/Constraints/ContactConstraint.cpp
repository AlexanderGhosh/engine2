#include "ContactConstraint.h"
#include "../../../Utils/BigMaths.h"
#include "../../../Componets/Rigidbody.h"
#include "../../../GameObject/GameObject.h"
#include "../../Engine.h"

Physics::Constraints::ContactConstraint::ContactConstraint(Component::RigidBody* a, Component::RigidBody* b) : Constraint(a, b), collisionData(nullptr)
{
    type = Type::Contact;
}

void Physics::Constraints::ContactConstraint::solve(const float& dt)
{
    const glm::vec3 normal = glm::normalize(collisionData->normal);
    const float& penetration = collisionData->penetration;
    const float baumgarte_slop = 0.001f;
    const float penetration_slop = fmin(penetration + baumgarte_slop, 0.0f);

    const float num_points = static_cast<float>(collisionData->points.size());
    const float num_points_inv = 1.0f / num_points;

    // Bias += -(BaumgarteScalar / Physics::Engine::getDeltaTime()) * penetration_slop;


    const BigMaths::mat12 inv_M(glm::mat3(body1->getInvMass()), body1->getInvInertia_G(),
        glm::mat3(body2->getInvMass()), body2->getInvInertia_G());

    BigMaths::vec12 V(body1->getVelocity(), body1->getAngularVelocity(),
        body2->getVelocity(), body2->getAngularVelocity());

    const float elasticity = E; 
    Jacobian = BigMaths::vec12(0);
    Jacobian.set(0, -normal);
    Jacobian.set(2, normal);
    BigMaths::vec12 delta_V(0);
    float lambda = 0;

    const float BaumgarteStabilizer = -(BaumgarteScalar / Physics::Engine::getDeltaTime()) * penetration;

    for (const glm::vec3& point : collisionData->points) {
        Bias = BaumgarteStabilizer;
        const glm::vec3 r1 = point - body1->getCOM();
        const glm::vec3 r2 = point - body2->getCOM();

        const float elatisity_term = glm::dot(normal,
            -body1->getVelocity() - glm::cross(body1->getAngularVelocity(), r1)
            + body2->getVelocity() + glm::cross(body2->getAngularVelocity(), r2));

        Bias += (elasticity * elatisity_term) * num_points;
        
        // --------------------LAMBDA-------------------- //
        //V.set(1, body1->getAngularVelocity());
        //V.set(3, body2->getAngularVelocity());
        //Jacobian.set(1, glm::cross(-r1, normal));
        //Jacobian.set(3, glm::cross(r2, normal));

        const float numerator = -(BigMaths::dot(Jacobian, V) + -BaumgarteStabilizer);
        const float demoninator = BigMaths::dot(Jacobian * inv_M, Jacobian);
        lambda = numerator / (demoninator == 0 ? 1 : demoninator);
        // --------------------LAMBDA-------------------- //

        auto x = (inv_M * Jacobian) * lambda;
        // delta_V = delta_V + x;
        body1->velocityAdder(x.get(0));
        body2->velocityAdder(x.get(2));

        auto r = BigMaths::dot(Jacobian, delta_V);
        r += Bias;
    }
}

void Physics::Constraints::ContactConstraint::cleanUp()
{
	collisionData = nullptr;
}

void Physics::Constraints::ContactConstraint::setCollisionData(CollisionManfold* data)
{
	collisionData = data;
    body2 = data->bodies[0]->getParent()->getRigidbody();
    body1 = data->bodies[1]->getParent()->getRigidbody();
}

const float Physics::Constraints::ContactConstraint::getLagrangian(Vector12 V, Matrix12 M, const CollisionManfold& manafoild) const
{
    const float b = getBias(manafoild);
    const float neumerator = -(BigMaths::dot(Jacobian, V) + b);
    const float denominator = BigMaths::dot(Jacobian * BigMaths::inverse(M), Jacobian); // the effective mass
    return neumerator / (!denominator ? 1 : denominator);
}

const float Physics::Constraints::ContactConstraint::getBias(const Physics::CollisionManfold& d) const
{
    return 0;
}

void Physics::Constraints::ContactConstraint::buildJacobian()
{
}
