#include "SphereDistribution.h"

Particles::SphereDisribution::SphereDisribution() : DistributionBase(), radius(0)
{
}

Particles::SphereDisribution::SphereDisribution(Float r) : SphereDisribution()
{
	this->radius = r;
}

const Particles::ParticleDistribution Particles::SphereDisribution::getDistribution() const
{
	return ParticleDistribution::Sphere;
}

void Particles::SphereDisribution::cleanUp()
{
}

void Particles::SphereDisribution::setRadius(Float r)
{
	radius = r;
}

glm::vec3 Particles::SphereDisribution::getPoint() const
{
	float rndRadius = Utils::random(0, radius);
	glm::vec3 res(0, 0, -rndRadius);
	glm::vec3 rotation(
		Utils::random(0, TWO_PI),
		Utils::random(0, TWO_PI), 
		Utils::random(0, TWO_PI)
	);
	glm::quat q(rotation);
	q = glm::normalize(q);
	res = q * res;
	return res;
}

Float Particles::SphereDisribution::getRadius() const
{
	return radius;
}
