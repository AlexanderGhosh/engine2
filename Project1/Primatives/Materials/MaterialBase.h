#pragma once
#include <glm.hpp>
#include <array>
#include <GL/glew.h>
#include "../../Utils/General.h"

namespace Materials {
	class MaterialBase {
	protected:
		MaterialBase();
	public:
		virtual void activateTextures(Int startUnit = 1) const = 0;
		virtual void cleanUp() = 0;
		~MaterialBase() = default;
	};

};


