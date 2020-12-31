#include "RigidBody.h"
#include "../Physics/Engine.h"
#include "../GameObject/GameObject.h"
#include "..\Physics\Constraints.h"

void Component::RigidBody::updateOrientation()
{
    *rotation = glm::normalize(*rotation);
    globalInverseIntertiaTensor =
        getRotation() 
        * localInverseIntertiaTensor
        * glm::inverse(getRotation());
}

void Component::RigidBody::addCollider(Physics::Collider* collider)
{
    // add collider to collider list
    colliders.push_back(collider);
    // reset local centroid & mass
    localCentroid = Utils::zero();
    mass = 0.0f;

    // compute local centroid & mass
    for (const Physics::Collider* collider : colliders)
    {
        // accumulate mass
        mass += collider->mass;

        // accumulate weighted contribution
        localCentroid +=
            collider->mass * collider->localCentroid;
    }

    // compute inverse mass
    inverseMass = 1.0f / mass;

    // compute final local centroid
    localCentroid *= inverseMass;

    // compute local inertia tensor
    glm::mat3 localInertiaTensor(0);
    for (const Physics::Collider* collider : colliders)
    {
        const glm::vec3 r = localCentroid - collider->localCentroid;
        const float rDotR = glm::dot(r, r); // magnitude squared
        const glm::mat3 rOutR = glm::outerProduct(r, r);

        // accumulate local inertia tensor contribution, 
        // using Parallel Axis Theorem
        localInertiaTensor +=
            collider->localInertiaTensor
            + collider->mass * rDotR * glm::mat3(1) - rOutR;
    }

    // compute inverse inertia tensor
    localInverseIntertiaTensor = glm::inverse(localInertiaTensor);
}



const glm::vec3 Component::RigidBody::localToGlobal(const glm::vec3& p) const
{
    return getRotation() * p + *position;
}

const glm::vec3 Component::RigidBody::globalToLocal(const glm::vec3& p) const
{
    return glm::inverse(getRotation()) * (p - *position);
}

const glm::vec3 Component::RigidBody::localToGlobalVec(const glm::vec3& v) const
{
    return getRotation() * v;
}

const glm::vec3 Component::RigidBody::globalToLocalVec(const glm::vec3& v) const
{
    return glm::inverse(getRotation()) * v;
}

void Component::RigidBody::applyForceAt(const glm::vec3& f, const glm::vec3& location)
{
    linearAcceleration += inverseMass * f;
    angularAcceleration += globalInverseIntertiaTensor * glm::cross(f, location - *position);
}

void Component::RigidBody::applyForce(const glm::vec3& f, const glm::vec3& ang)
{
    linearAcceleration += inverseMass * f;
    angularAcceleration += globalInverseIntertiaTensor  * ang;
}

void Component::RigidBody::applyAccAt(const glm::vec3& f, const glm::vec3& at)
{
    linearAcceleration += f;
    angularAcceleration += glm::cross(f, at - *position);
}

void Component::RigidBody::applyAcceleration(const glm::vec3& f, const glm::vec3& ang)
{
    linearAcceleration += f;
    angularAcceleration += ang;
}

Utils::BigMaths::MassMatrix6 Component::RigidBody::getMassMatrix() const
{
    Utils::BigMaths::MassMatrix6 res{};
    const glm::mat3 m(mass);
    const glm::mat3& I = this->localInverseIntertiaTensor;
    for (char i = 0; i < 3; i++) {
        for (char j = 0; j < 3; j++) {
            res[i][j] = m[i][j];
            res[3 + i][3 + j] = I[i][j] ? 1.0f / I[i][j] : 0.0f;
        }
    }
    return res;
}

Utils::BigMaths::Vector6 Component::RigidBody::getVelocities() const
{
    const glm::vec3& l = this->linearVelocity;
    const glm::vec3& a = this->angularVelocity;
    return { l.x, l.y, l.z, a.x, a.y, a.z };
}

Component::RigidBody::RigidBody() : Base(), mass(0), inverseMass(0),
globalInverseIntertiaTensor(0), 
localInverseIntertiaTensor(0), globalCentroid(0), localCentroid(0),
position(nullptr), rotation(nullptr), linearVelocity(0), angularVelocity(0),
colliders(), linearAcceleration(0), angularAcceleration(0)
{
    Physics::Engine::addRigidBody(this);
}

void Component::RigidBody::update()
{
    if (isKinimatic) {
        linearAcceleration = Utils::zero();
        angularAcceleration= Utils::zero();
        linearVelocity = Utils::zero();
        angularVelocity = Utils::zero();
    }
    const float dt = 1.0f / 60.0f;
    linearVelocity += linearAcceleration * dt;
    angularVelocity += angularAcceleration * dt;

    linearAcceleration = Utils::zero();
    angularAcceleration = Utils::zero();

    globalCentroid += linearVelocity * dt;
    *position += linearVelocity * dt;

    *rotation *= glm::quat(angularVelocity * dt);

    // update physical properties
    updateOrientation();
    // updatePositionFromGlobalCentroid();
}

void Component::RigidBody::setParent(GameObject* parent)
{
    Base::setParent(parent);
    Component::Transform* trans = parent->getTransform();
    position = &trans->Position;
    rotation = &trans->Rotation;
}
