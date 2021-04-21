#pragma once
#include "DistributionBase.h"
namespace Particles {
	class ConeDistribution : public DistributionBase
	{
	private:
		float height, radius;
		float r_div_h;
	public:
		ConeDistribution();
		ConeDistribution(Float h, Float r);
		~ConeDistribution() = default;
		const ParticleDistribution getDistribution() const;
		void cleanUp();

		// setters
		void setHeight(Float h);
		void setRadius(Float r);

		// getters
		glm::vec3 getPoint() const;
		glm::vec3 getNormalised() const;
		Float getHeight() const;
		Float getRadius() const;
	};
};

