#include "DomeDisribution.h"

Particles::DomeDisribution::DomeDisribution() : DistributionBase(), radius(0)
{
}

Particles::DomeDisribution::DomeDisribution(Float r) : DomeDisribution()
{
	this->radius = r;
}

const Particles::ParticleDistribution Particles::DomeDisribution::getDistribution() const
{
	return ParticleDistribution::Dome;
}

void Particles::DomeDisribution::cleanUp()
{
}

void Particles::DomeDisribution::setRadius(Float r)
{
	radius = r;
}

glm::vec3 Particles::DomeDisribution::getPoint() const
{
	float rndRadius = Utils::random(0, radius);
	glm::vec3 res(0, rndRadius, 0);
	glm::vec3 rotation(
		Utils::random(0, PI),
		Utils::random(0, PI),
		Utils::random(0, PI)
	);
	glm::quat q(rotation);
	q = glm::normalize(q);
	res = q * res;
	return res;
}

Float Particles::DomeDisribution::getRadius() const
{
	return radius;
}
