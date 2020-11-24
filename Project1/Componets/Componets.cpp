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

const glm::vec3 Component::Base::getCenter() const
{
    return parent->getTransform()->Position;
}
