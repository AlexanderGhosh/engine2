#pragma once
#include "LightBase.h"

namespace Component {
	class PointLight : public LightBase
	{
	public:
		PointLight();
		PointLight(Vector3 colour, Float brightness = 1);
		~PointLight() = default;
		LightType getLightType() const;
		void cleanUp();
	};
}

