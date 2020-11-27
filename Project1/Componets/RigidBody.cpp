#include "RigidBody.h"
#include "../Physics/Engine.h"
#include "../GameObject/GameObject.h"
#include "..\Physics\Constraints.h"

void Component::RigidBody::updateGlobalCentroidFromPosition()
{
	globalCentroid = getRotation() * localCentroid + *position;
}

void Component::RigidBody::updatePositionFromGlobalCentroid()
{
	// *position = getRotation() * -localCentroid + globalCentroid;
}

void Component::RigidBody::updateOrientation()
{
    *rotation = glm::normalize(*rotation);
    globlaInverseIntertiaTensor =
        getRotation() 
        * glm::inverse(localInverseIntertiaTensor)
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
            + collider->mass * (rDotR * glm::mat3(1) - rOutR);
    }

    // compute inverse inertia tensor
    glm::inverse(localInertiaTensor);
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

void Component::RigidBody::applyForce(const glm::vec3& f, const glm::vec3& at)
{
    forceAccumulator += f / FPS;
    torqueAccumulator += glm::cross(at - globalCentroid, f / FPS);
}

Utils::BigMaths::MassMatrix6 Component::RigidBody::getMassMatrix() const
{
    Utils::BigMaths::MassMatrix6 res{};
    const glm::mat3 m(mass);
    const glm::mat3& I = this->localInverseIntertiaTensor;
    for (char i = 0; i < 3; i++) {
        for (char j = 0; j < 3; j++) {
            res[i][j] = m[i][j];
            res[3 + i][3 + j] = 1.0f / I[i][j];
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
globlaInverseIntertiaTensor(0), 
localInverseIntertiaTensor(0), globalCentroid(0), localCentroid(0),
position(nullptr), rotation(nullptr), linearVelocity(0), angularVelocity(0),
colliders(), forceAccumulator(0), torqueAccumulator(0)
{
    Physics::Engine::addRigidBody(this);
}

void Component::RigidBody::update()
{
    if (kinematic)
        return;
    const float dt = 1.0f / 60.0f;
    linearVelocity += inverseMass * forceAccumulator;
    angularVelocity += globlaInverseIntertiaTensor * torqueAccumulator;

    forceAccumulator = Utils::zero();
    torqueAccumulator = Utils::zero();

    globalCentroid += linearVelocity * dt;
    *position += linearVelocity * dt;

    const glm::vec3 axis = glm::normalize(angularVelocity);
    const float angle = glm::length(angularVelocity) * dt;
    // *rotation = glm::toMat3(glm::quat(axis.x, axis.y, axis.z, angle)) * getRotation();

    // update physical properties
    // updateOrientation();
    updatePositionFromGlobalCentroid();
}

void Component::RigidBody::setParent(GameObject* parent)
{
    Base::setParent(parent);
    Component::Transform* trans = parent->getTransform();
    position = &trans->Position;
    rotation = &trans->Rotation;
}
