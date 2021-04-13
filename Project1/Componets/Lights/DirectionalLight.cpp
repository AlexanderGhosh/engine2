#include "DirectionalLight.h"

Component::DirectionalLight::DirectionalLight() : LightBase(), direction(0)
{
}

Component::LightType Component::DirectionalLight::getLightType() const
{
	return LightType::Directional;
}
