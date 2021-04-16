#include "LightBase.h"
#include "../../GameObject/GameObject.h"

Component::LightBase::LightBase() : ComponetBase(), position(nullptr), colour(0), brightness(0)
{
}

Component::LightBase::LightBase(Vector3 colour, Float brightness) : LightBase()
{
	this->colour = colour;
	this->brightness = brightness;
}

void Component::LightBase::setParent(GameObject* parent)
{
	ComponetBase::setParent(parent);
	position = &parent->getTransform()->Position;
}

void Component::LightBase::cleanUp()
{
	position = nullptr;
}

Vector3 Component::LightBase::getPosition() const
{
	return *position;
}

Vector3 Component::LightBase::getColour() const
{
	return colour;
}

Float Component::LightBase::getBrightness() const
{
	return brightness;
}

