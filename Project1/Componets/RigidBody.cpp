#include "RigidBody.h"

void Componet::RigidBody::updateGlobalCentroidFromPosition()
{
	globalCentroid = getRotation() * localCentroid + *position;
}

void Componet::RigidBody::updatePositionFromGlobalCentroid()
{
	*position = getRotation() * -localCentroid + globalCentroid;
}

void Componet::RigidBody::updateOrientation()
{
    *rotation = glm::normalize(*rotation);
    globlaInverseIntertiaTensor =
        getRotation() 
        * glm::inverse(localInverseIntertiaTensor)
        * glm::inverse(getRotation());
}

void Componet::RigidBody::addCollider(Physics::Collider& collider)
{
    // add collider to collider list
    colliders.push_back(&collider);

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

const glm::vec3 Componet::RigidBody::localToGlobal(const glm::vec3& p) const
{
    return getRotation() * p + *position;
}

const glm::vec3 Componet::RigidBody::globalToLocal(const glm::vec3& p) const
{
    return glm::inverse(getRotation()) * (p - *position);
}

const glm::vec3 Componet::RigidBody::localToGlobalVec(const glm::vec3& v) const
{
    return getRotation() * v;
}

const glm::vec3 Componet::RigidBody::globalToLocalVec(const glm::vec3& v) const
{
    return glm::inverse(getRotation()) * v;
}

void Componet::RigidBody::applyForce(const glm::vec3& f, const glm::vec3& at)
{
    forceAccumulator += f;
    torqueAccumulator += glm::cross(at - globalCentroid, f);
}

void Componet::RigidBody::fixedUpdate()
{
    const float dt = 1.0f / 60.0f;
    linearVelocity += inverseMass * forceAccumulator * dt;
    angularVelocity += globlaInverseIntertiaTensor * torqueAccumulator * dt;

    forceAccumulator = Utils::zero();
    torqueAccumulator = Utils::zero();

    globalCentroid += linearVelocity * dt;

    const glm::vec3 axis = glm::normalize(angularVelocity);
    const float angle = glm::length(angularVelocity) * dt;
    *rotation = glm::toMat3(glm::quat(axis.x, axis.y, axis.z, angle)) * getRotation();

    // update physical properties
    updateOrientation();
    updatePositionFromGlobalCentroid();
}
