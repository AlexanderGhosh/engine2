#include "SpotLight.h"

Component::SpotLight::SpotLight() : LightBase()
{
}

Component::LightType Component::SpotLight::getLightType() const
{
	return LightType::Spot;
}
void Component::SpotLight::cleanUp()
{
	LightBase::cleanUp();
}
