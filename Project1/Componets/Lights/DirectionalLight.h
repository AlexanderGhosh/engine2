#pragma once
#include "LightBase.h"

namespace Component {
	class DirectionalLight : public LightBase
	{
	private:
		glm::vec3 direction;
	public:
		DirectionalLight();
		~DirectionalLight() = default;
		LightType getLightType() const;
	};
}

