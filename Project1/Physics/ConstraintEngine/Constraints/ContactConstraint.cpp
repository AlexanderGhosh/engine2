#include "ContactConstraint.h"
#include "../../../Componets/Rigidbody.h"

Physics::Constraints::ContactConstraint::ContactConstraint(Component::RigidBody* a, Component::RigidBody* b) : Constraint(a, b)
{
    type = Type::Contact;
}

void Physics::Constraints::ContactConstraint::solve(const float& dt)
{
    MATHS::Vector12 h{ };
    const float lambda = 1/*getLagrangian(V, M, *collisionData)*/;
    const glm::vec3& n = collisionData->normal;
    const auto inv_m = MATHS::inverse({ } /*M*/);
    glm::vec3 acc(0);
    for (const glm::vec3& point : collisionData->points) {
        glm::vec3 temp1 = -glm::cross(point - body1->getCOS(), n);
        glm::vec3 temp2 = glm::cross(point - body2->getCOS(), n);
        Jacobian = {
            -n.x, -n.y, -n.z,
            temp1.x, temp1.y, temp1.z,
            n.x, n.y, n.z,
            temp2.x, temp2.y, temp2.z
        };
        Jacobian = (inv_m * Jacobian) * lambda / static_cast<float>(collisionData->points.size());
        auto split = MATHS::split(Jacobian);
        acc += MATHS::split(split[0])[0] / static_cast<float>(collisionData->points.size());
        // manafold.bodies[0]->getParent()->getRigidbody()->applyForceAt(-MATHS::split(split[0])[0] / static_cast<float>(manafold.points.size()), *manafold.bodies[0]->position);
        // manafold.bodies[1]->getParent()->getRigidbody()->applyForceAt( MATHS::split(split[1])[0] / static_cast<float>(manafold.points.size()), *manafold.bodies[1]->position);

        //h = h + (inv_m * Jacobian * lambda) / manafold.points.size();
    }
    body1->velocityAdder(acc);
    body2->velocityAdder(-acc);
    std::cout << glm::to_string(acc) << std::endl;
    // float test = (V + h) * Jacobian + getBias(manafold);
    // test = (int)(test * 10000.0) / 10000.0;
    // assert(test == 0 && "constraint != 0");
    //return Jacobian * lambda;
}

void Physics::Constraints::ContactConstraint::cleanUp()
{
	delete collisionData;
	collisionData = nullptr;
}

void Physics::Constraints::ContactConstraint::setCollisionData(CollisionManfold* data)
{
	collisionData = data;
}

void Physics::Constraints::ContactConstraint::buildJacobian()
{
}
