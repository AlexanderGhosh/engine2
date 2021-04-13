#include "LightBase.h"
#include "../../GameObject/GameObject.h"

Component::LightBase::LightBase() : ComponetBase(), position(nullptr), colour(0)
{
}

void Component::LightBase::setParent(GameObject* parent)
{
	ComponetBase::setParent(parent);
	position = &parent->getTransform()->Position;
}
