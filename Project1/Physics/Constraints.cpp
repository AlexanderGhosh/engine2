#include "Constraints.h"
#include "../Componets/RigidBody.h"
#include "Collision/CollisionDetection.h"
#include "../GameObject/GameObject.h"


Utils::BigMaths::Vector12 Physics::Constraint::getDeltaV(const MATHS::Vector12& V, const MATHS::Matrix12& M, Physics::CollisionManfold& manafoild)
{
    const glm::vec3& n = manafoild.normal;
    glm::vec3 temp1 = -glm::cross(manafoild.getDeltaA(), manafoild.normal);
    glm::vec3 temp2 = glm::cross(manafoild.getDeltaB(), manafoild.normal);
    Jacobian = { 
        -n.x, -n.y, -n.z, 
        temp1.x, temp1.y, temp1.z, 
        n.x, n.y, n.z,
        temp2.x, temp2.y, temp2.z 
    };

    const auto inv_m = Utils::BigMaths::inverse(M);

    const float lambda = getLagrangian(V, M, manafoild);
    auto m = inv_m;
    MATHS::Vector12 h = m * Jacobian;
    h = h * lambda;
    float test = (V + h) * Jacobian + getBias(manafoild);
    test = (int)(test * 10000.0) / 10000.0;
    assert(test == 0);
    return Jacobian * lambda * FPS;
};
const float Physics::Constraint::getLagrangian(const MATHS::Vector12& V, const MATHS::Matrix12& M, const CollisionManfold& manafoild) const
{
    float b = getBias(manafoild);
    const float neumerator = -((Jacobian * V) + b);
    const float denominator = (Jacobian * MATHS::inverse(M)) * Jacobian; // the effective mass
    return neumerator / (denominator == 0 ? 1 : denominator);

}
const float Physics::Constraint::getBias(const Physics::CollisionManfold& manafold) const {
    //return 0;
    Component::RigidBody* a = manafold.bodies[0]->getParent()->getRigidbody();
    Component::RigidBody* b = manafold.bodies[1]->getParent()->getRigidbody();
    const Utils::BigMaths::Vector6& aV = a->getVelocities();
    const Utils::BigMaths::Vector6& bV = a->getVelocities();
    auto t = manafold.getRestitution() *
        glm::dot(-glm::vec3(aV[0], aV[1], aV[2]) - glm::cross({aV[0], aV[1], aV [2] }, manafold.getDeltaA()) +
            glm::vec3(bV[0], bV[1], bV[2]) + glm::cross(glm::vec3(bV[0], bV[1], bV[2]), manafold.getDeltaB()), manafold.normal);
    return -(BETA * FPS) * manafold.penertraion;
}
