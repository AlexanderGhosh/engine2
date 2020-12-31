#include "Constraints.h"
#include "../Componets/RigidBody.h"
#include "Collision/CollisionDetection.h"
#include "../GameObject/GameObject.h"


void Physics::Constraint::solve(const MATHS::Vector12& V, const MATHS::Matrix12& M, Physics::CollisionManfold& manafold)
{
    MATHS::Vector12 h;
    const float lambda = getLagrangian(V, M, manafold);
    const glm::vec3& n = manafold.normal;
    const auto inv_m = MATHS::inverse(M);
    glm::vec3 acc(0);
    for (const glm::vec3& point : manafold.points) {
        glm::vec3 temp1 = -glm::cross(point - *manafold.bodies[0]->position, n);
        glm::vec3 temp2 = glm::cross(point - *manafold.bodies[1]->position, n);
        Jacobian = {
            -n.x, -n.y, -n.z,
            temp1.x, temp1.y, temp1.z,
            n.x, n.y, n.z,
            temp2.x, temp2.y, temp2.z
        };
        Jacobian = (inv_m * Jacobian) * lambda / static_cast<float>(manafold.points.size());
        auto split = MATHS::split(Jacobian);
        acc += MATHS::split(split[0])[0] / static_cast<float>(manafold.points.size());
        // manafold.bodies[0]->getParent()->getRigidbody()->applyForceAt(-MATHS::split(split[0])[0] / static_cast<float>(manafold.points.size()), *manafold.bodies[0]->position);
        // manafold.bodies[1]->getParent()->getRigidbody()->applyForceAt( MATHS::split(split[1])[0] / static_cast<float>(manafold.points.size()), *manafold.bodies[1]->position);

        //h = h + (inv_m * Jacobian * lambda) / manafold.points.size();
    }
    auto b = manafold.bodies[1]->getParent()->getRigidbody();
    manafold.bodies[0]->getParent()->getRigidbody()->linearVelocity += (acc);
    manafold.bodies[1]->getParent()->getRigidbody()->linearVelocity += (-acc);
    std::cout << glm::to_string(acc) << std::endl;
    // float test = (V + h) * Jacobian + getBias(manafold);
    // test = (int)(test * 10000.0) / 10000.0;
    // assert(test == 0 && "constraint != 0");
    //return Jacobian * lambda;
};
const float Physics::Constraint::getLagrangian(const MATHS::Vector12& V, const MATHS::Matrix12& M, const CollisionManfold& manafoild) const
{
    const float b = getBias(manafoild);
    const float neumerator = -((Jacobian * V) + b);
    const float denominator = (Jacobian * MATHS::inverse(M)) * Jacobian; // the effective mass
    return neumerator / (!denominator ? 1 : denominator);

}
const float Physics::Constraint::getBias(const Physics::CollisionManfold& manafold) const {
    /*Component::RigidBody* a = manafold.bodies[0]->getParent()->getRigidbody();
    Component::RigidBody* b = manafold.bodies[1]->getParent()->getRigidbody();
    const Utils::BigMaths::Vector6& aV = a->getVelocities();
    const Utils::BigMaths::Vector6& bV = a->getVelocities();
    auto t = manafold.getRestitution() *
        glm::dot(-glm::vec3(aV[0], aV[1], aV[2]) - glm::cross({aV[0], aV[1], aV [2] }, manafold.getDeltaA()) +
            glm::vec3(bV[0], bV[1], bV[2]) + glm::cross(glm::vec3(bV[0], bV[1], bV[2]), manafold.getDeltaB()), manafold.normal);*/
    return -(BETA * FPS) * manafold.penetration;
}
