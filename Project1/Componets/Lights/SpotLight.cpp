#include "SpotLight.h"

Component::SpotLight::SpotLight() : LightBase(), direction(0), cutOff(0), outterCutOff(0), constant(0), linear(0), quadratic(0)
{
}

Component::SpotLight::SpotLight(Vector3 dir, Float cutOff, Float outterCutOff, Float constant, Float linear, Float quadratic) : SpotLight()
{
	direction = dir;
	this->cutOff = cutOff;
	this->outterCutOff = outterCutOff;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

Component::LightType Component::SpotLight::getLightType() const
{
	return LightType::Spot;
}

void Component::SpotLight::cleanUp()
{
	LightBase::cleanUp();
}

void Component::SpotLight::setDirection(Vector3 dir)
{
	direction = dir;
}

void Component::SpotLight::setCutOff(Float cutOff)
{
	this->cutOff = cutOff;
}

void Component::SpotLight::setOutterCutOff(Float outterCutOff)
{
	this->outterCutOff = outterCutOff;
}

void Component::SpotLight::setConstant(Float constant)
{
	this->constant = constant;
}

void Component::SpotLight::setLinear(Float linear)
{
	this->linear = linear;
}

void Component::SpotLight::setQuadratic(Float quadratic)
{
	this->quadratic = quadratic;
}

Vector3 Component::SpotLight::getDirection() const
{
	return direction;
}

Float Component::SpotLight::getCutOff() const
{
	return cutOff;
}

Float Component::SpotLight::getOutterCutOff() const
{
	return outterCutOff;
}

Float Component::SpotLight::getConstant() const
{
	return constant;
}

Float Component::SpotLight::getLinear() const
{
	return linear;
}

Float Component::SpotLight::getQuadratic() const
{
	return quadratic;
}
