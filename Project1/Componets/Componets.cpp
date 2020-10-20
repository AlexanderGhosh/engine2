#include "Componets.h"

namespace Componet {
    const glm::mat4 Transform::getModel() const
    {
        glm::mat4 model(1);
        model = glm::translate(model, Position);
        model = Utils::rotate(model, Rotation);
        model = glm::scale(model, Scale);
        return model;
    }
}
