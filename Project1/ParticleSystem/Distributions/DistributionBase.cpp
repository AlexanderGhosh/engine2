#include "DistributionBase.h"

Particles::DistributionBase::DistributionBase()
{
}

glm::vec3 Particles::DistributionBase::getNormalised() const
{
    return glm::normalize(getPoint());
}
