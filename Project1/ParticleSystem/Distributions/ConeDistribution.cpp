#include "ConeDistribution.h"

Particles::ConeDistribution::ConeDistribution() : DistributionBase(), height(0), radius(0), r_div_h(1)
{
}

Particles::ConeDistribution::ConeDistribution(Float h, Float r) : ConeDistribution()
{
	this->height = h;
	this->radius = r;
	r_div_h = radius / height;
}

const Particles::ParticleDistribution Particles::ConeDistribution::getDistribution() const
{
	return ParticleDistribution::Cone;
}

void Particles::ConeDistribution::cleanUp()
{
}

void Particles::ConeDistribution::setHeight(Float h)
{
	height = h;
}

void Particles::ConeDistribution::setRadius(Float r)
{
	radius = r;
}

glm::vec3 Particles::ConeDistribution::getPoint() const
{
	glm::vec3 res(0);
	const float h = height * powf(Utils::random(), 0.333333f);
	const float r = r_div_h * h * powf(Utils::random(), 0.5f);
	const float t = TWO_PI * Utils::random();

	res.x = r * cosf(t);
	res.y = h;
	res.z = r * sinf(t);
	return res;
}

glm::vec3 Particles::ConeDistribution::getNormalised() const
{
	return glm::normalize(getPoint());
}

Float Particles::ConeDistribution::getHeight() const
{
	return height;
}

Float Particles::ConeDistribution::getRadius() const
{
	return radius;
}
