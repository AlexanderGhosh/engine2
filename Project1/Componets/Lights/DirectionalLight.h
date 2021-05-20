#pragma once
#include "LightBase.h"

namespace Component {
	class DirectionalLight : public LightBase
	{
	private:
		glm::vec3 direction;
	public:
		DirectionalLight();
		DirectionalLight(Vector3 direction, Vector3 colour, Float brightness = 1);
		~DirectionalLight() = default;
		LightType getLightType() const;
		void cleanUp();
		Vector3 getDirection() const;
		Byte_Array getByteField() const;
	};
}

