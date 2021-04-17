#include "PointLight.h"

Component::PointLight::PointLight() : LightBase()
{
}

Component::PointLight::PointLight(Vector3 colour, Float brightness) : LightBase(colour, brightness)
{
}

Component::LightType Component::PointLight::getLightType() const
{
	return LightType::Point;
}

void Component::PointLight::cleanUp()
{
	LightBase::cleanUp();
}
