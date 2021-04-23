#include "DomeDistribution.h"

Particles::DomeDistribution::DomeDistribution() : DistributionBase(), radius(0)
{
}

Particles::DomeDistribution::DomeDistribution(Float r) : DomeDistribution()
{
	this->radius = r;
}

const Particles::ParticleDistribution Particles::DomeDistribution::getDistribution() const
{
	return ParticleDistribution::Dome;
}

void Particles::DomeDistribution::cleanUp()
{
}

void Particles::DomeDistribution::setRadius(Float r)
{
	radius = r;
}

glm::vec3 Particles::DomeDistribution::getPoint() const
{
	float rndRadius = Utils::random(0, radius);
	glm::vec3 res(0, rndRadius, 0);
	glm::vec3 rotation(0);
	do {
		rotation.x = Utils::random(0, TWO_PI);
		rotation.y = Utils::random(0, TWO_PI);
		rotation.z = Utils::random(0, TWO_PI);
	} 	while (rotation.y < 0);
	glm::quat q(rotation);
	q = glm::normalize(q);
	res = q * res;
	res.y = fabsf(res.y);
	return res;
}

Float Particles::DomeDistribution::getRadius() const
{
	return radius;
}
