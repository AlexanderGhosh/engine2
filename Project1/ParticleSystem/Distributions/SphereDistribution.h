#pragma once
#include "DistributionBase.h"
namespace Particles {
	class SphereDisribution : public DistributionBase
	{
	private:
		float radius;
	public:
		SphereDisribution();
		SphereDisribution(Float r);
		~SphereDisribution() = default;
		const ParticleDistribution getDistribution() const;
		void cleanUp();

		// setters
		void setRadius(Float r);

		// getters
		glm::vec3 getPoint() const;
		Float getRadius() const;
	};
}
