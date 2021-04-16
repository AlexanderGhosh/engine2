#pragma once
#include "LightBase.h"

namespace Component {
	class SpotLight : public LightBase
	{
	public:
		SpotLight();
		~SpotLight() = default;
		LightType getLightType() const;
		void cleanUp();
	};
}

