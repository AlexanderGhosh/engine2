#include "ComponetBase.h"
#include "../GameObject/GameObject.h"

const glm::mat4 Component::Transform::getModel() const
{
    glm::mat4 model(1);
    model = glm::translate(model, Position);
    model = Utils::rotate(model, Rotation);
    model = glm::scale(model, Scale);
    return model;
}

Component::Transform::Transform(glm::vec3 p, glm::vec3 s, glm::vec3 r) : ComponetBase(), Position(p), Scale(s), Rotation(r)
{
}

void Component::Transform::rotate(Vector3 axis, Float angle)
{
    rotate(axis * angle);
}

void Component::Transform::rotate(Vector3 rotation)
{
    Rotation *= glm::normalize(glm::quat(glm::vec3(rotation.x, rotation.y, rotation.z)));
    Rotation = glm::normalize(Rotation);
}

void Component::Transform::cleanUp()
{
}

Byte_Array Component::Transform::getByteField() const
{
    Byte_Array res;
    res.reserve(ByteSizeOfComponents[static_cast<int>(getType())]);

    Utils::addToByteArray(res, Position);

    Utils::addToByteArray(res, Scale);

    glm::vec3 eulerRot = glm::degrees(glm::eulerAngles(Rotation));
    Utils::addToByteArray(res, eulerRot);

    return res;
}

Component::Transform Component::Transform::operator+(const Transform& a) const
{
    Transform res(*this);
    res.Position += a.Position;
    res.Scale *= a.Scale;
    res.Rotation *= a.Rotation;
    res.Rotation = glm::normalize(res.Rotation);
    return res;
}

Component::Transform& Component::Transform::operator+=(const Transform& a)
{
    Position += a.Position;
    Scale *= a.Scale;
    Rotation *= a.Rotation;
    Rotation = glm::normalize(Rotation);
    return *this;
}
