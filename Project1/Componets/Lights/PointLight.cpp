#include "PointLight.h"

Component::PointLight::PointLight() : LightBase()
{
}

Component::LightType Component::PointLight::getLightType() const
{
	return LightType::Point;
}