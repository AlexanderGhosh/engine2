#pragma once
#include <GL/glew.h>
#include "../../Utils/General.h"

namespace Materials {
	class MatItem;
	class Material;
	class Forward;
	class PBR;
}
namespace Render {
	namespace Animation {
		struct KeyFrame;
	}
	namespace Shading {
		class Manager
		{
		public:
			static bool setValue(String name, int val);
			static bool setValue(String name, float val);
			static bool setValue(String name, glm::vec2 val);
			static bool setValue(String name, glm::vec3 val);
			static bool setValue(String name, glm::vec4 val);
			static bool setValue(String name, glm::mat3 val);
			static bool setValue(String name, glm::mat4 val);
			static bool setValue(String name, const Materials::MatItem& fwd, int& texUnit);
			static bool setValue(String name, const Materials::Material*mat, int unit);
			static bool setValue(String name, const Render::Animation::KeyFrame& frame);
			static bool setValue(String name, const Materials::Forward& fwd);
			static bool setValue(String name, const Materials::PBR& mat, int unit);
			static void setActive(Unsigned shaderId);
			static void cleanUp();
		private:
			static unsigned active;
		};
	}
}

