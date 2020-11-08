#include "Componets.h"

const glm::mat4 Componet::Transform::getModel() const
{
    glm::mat4 model(1);
    model = glm::translate(model, Position);
    model = Utils::rotate(model, Rotation);
    model = glm::scale(model, Scale);
    return model;
}
