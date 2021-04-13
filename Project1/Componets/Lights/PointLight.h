#pragma once
#include "LightBase.h"

namespace Component {
	class PointLight : public LightBase
	{
	public:
		PointLight();
		~PointLight() = default;
		LightType getLightType() const;
	};
}

