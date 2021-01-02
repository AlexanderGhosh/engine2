#include "Constraints.h"
#include "../../../Componets/RigidBody.h"
#include "../../Collision/CollisionDetection.h"
#include "../../../GameObject/GameObject.h"
#include "../ConstraitnsSolver.h"

Physics::Constraints::Constraint::Constraint(): Jacobian(), enabled(1), type(Type::None), body1(nullptr), body2(nullptr) {
    // ConstraintsSolver::addConstraint(this);
}
Physics::Constraints::Constraint::Constraint(Component::RigidBody* a, Component::RigidBody* b) : Constraint()
{
    this->body1 = body1;
    this->body2 = body2;
};
void Physics::Constraints::Constraint::preSolve()
{
}
void Physics::Constraints::Constraint::setBodyA(Component::RigidBody* a)
{
    body1 = a;
}
void Physics::Constraints::Constraint::setBodyB(Component::RigidBody* b)
{
    body2 = b;
}
void Physics::Constraints::Constraint::setBodies(Component::RigidBody* a, Component::RigidBody* b)
{
    body1 = a;
    body2 = b;
}
;
const float Physics::Constraints::Constraint::getLagrangian(const MATHS::Vector12& V, const MATHS::Matrix12& M, const CollisionManfold& manafoild) const
{
    const float b = getBias(manafoild);
    const float neumerator = -((Jacobian * V) + b);
    const float denominator = (Jacobian * MATHS::inverse(M)) * Jacobian; // the effective mass
    return neumerator / (!denominator ? 1 : denominator);

}
const float Physics::Constraints::Constraint::getBias(const Physics::CollisionManfold& manafold) const {
    /*Component::RigidBody* a = manafold.bodies[0]->getParent()->getRigidbody();
    Component::RigidBody* b = manafold.bodies[1]->getParent()->getRigidbody();
    const Utils::BigMaths::Vector6& aV = a->getVelocities();
    const Utils::BigMaths::Vector6& bV = a->getVelocities();
    auto t = manafold.getRestitution() *
        glm::dot(-glm::vec3(aV[0], aV[1], aV[2]) - glm::cross({aV[0], aV[1], aV [2] }, manafold.getDeltaA()) +
            glm::vec3(bV[0], bV[1], bV[2]) + glm::cross(glm::vec3(bV[0], bV[1], bV[2]), manafold.getDeltaB()), manafold.normal);*/
    return -(1 * 60) * manafold.penetration;
}
