#pragma once
#include "DistributionBase.h"
namespace Particles {
	class DomeDistribution : public DistributionBase
	{
	private:
		float radius;
	public:
		DomeDistribution();
		DomeDistribution(Float r);
		~DomeDistribution() = default;
		const ParticleDistribution getDistribution() const;
		void cleanUp();

		// setters
		void setRadius(Float r);

		// getters
		glm::vec3 getPoint() const;
		Float getRadius() const;
	};
}
