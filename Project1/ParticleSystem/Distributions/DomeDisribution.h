#pragma once
#include "DistributionBase.h"
namespace Particles {
	class DomeDisribution : public DistributionBase
	{
	private:
		float radius;
	public:
		DomeDisribution();
		DomeDisribution(Float r);
		~DomeDisribution() = default;
		const ParticleDistribution getDistribution() const;
		void cleanUp();

		// setters
		void setRadius(Float r);

		// getters
		glm::vec3 getPoint() const;
		Float getRadius() const;
	};
}
