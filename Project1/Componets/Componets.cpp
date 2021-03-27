#include "Componets.h"
#include "../GameObject/GameObject.h"

const glm::mat4 Component::Transform::getModel() const
{
    glm::mat4 model(1);
    model = glm::translate(model, Position);
    model = Utils::rotate(model, Rotation);
    model = glm::scale(model, Scale);
    return model;
}
/// <summary>
/// angle in radians
/// </summary>
/// <param name="axis"></param>
/// <param name="angle"></param>
void Component::Transform::rotate(Vector3 axis, Float angle)
{
    rotate(axis * angle);
}
/// <summary>
/// angle in radians
/// </summary>
/// <param name="rotation"></param>
void Component::Transform::rotate(Vector3 rotation)
{
    Rotation *= glm::normalize(glm::quat(glm::vec3(rotation.x, rotation.y, rotation.z)));
    Rotation = glm::normalize(Rotation);
}
