#pragma once
#include "../../Utils/General.h"
namespace Particles {
	enum class ParticleDistribution {
		Cone, Sphere, Dome
	};
	class DistributionBase
	{
	protected:
		DistributionBase();
	public:
		~DistributionBase() = default;
		virtual const ParticleDistribution getDistribution() const = 0;
		virtual void cleanUp() = 0;
		virtual glm::vec3 getPoint() const = 0;
		virtual glm::vec3 getNormalised() const;
	};
};

