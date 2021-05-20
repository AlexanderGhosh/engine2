#include "DirectionalLight.h"

Component::DirectionalLight::DirectionalLight() : LightBase(), direction(0)
{
}

Component::DirectionalLight::DirectionalLight(Vector3 direction, Vector3 colour, Float brightness) : DirectionalLight()
{
	this->direction = direction;
	this->colour = colour;
	this->brightness = brightness;
}

Component::LightType Component::DirectionalLight::getLightType() const
{
	return LightType::Directional;
}
void Component::DirectionalLight::cleanUp()
{
	LightBase::cleanUp();
}

Vector3 Component::DirectionalLight::getDirection() const
{
	return direction;
}

Byte_Array Component::DirectionalLight::getByteField() const
{
	Byte_Array res = LightBase::getByteField();
	Utils::addToByteArray(res, direction);
	return res;
}
