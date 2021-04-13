#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "../../Primatives/Materials/MatItemBase.h"

namespace Materials {
	class MaterialBase;
	class Forward;
	class PBR;
}
namespace Component {
	class LightBase;
}
namespace Render {
	namespace Animation {
		struct KeyFrame;
	}
	namespace Shading {
		class Manager
		{
		public:
			// primatives
			static bool setValue(String name, int val);
			static bool setValue(String name, float val);
			// vectors
			static bool setValue(String name, glm::vec2 val);
			static bool setValue(String name, glm::vec3 val);
			static bool setValue(String name, glm::vec4 val);
			static bool setValue(String name, glm::mat3 val);
			static bool setValue(String name, glm::mat4 val);
			// complex
			// MatItem
			template<class T>
			static inline bool setValue(String name, const Materials::MatItemBase<T>& item, unsigned& texUnit) {
				bool valid = true;
				valid = Render::Shading::Manager::setValue(name + ".id", static_cast<int>(texUnit)) AND valid; // set to texture unit
				valid = Render::Shading::Manager::setValue(name + ".raw", item.getCurrentRaw()) AND valid; // set the raw value
				valid = Render::Shading::Manager::setValue(name + ".mixValue", item.getCurrentMixValue()) AND valid; // set the mix value
				valid = Render::Shading::Manager::setValue(name + ".repeatValue", item.getRepeatValue()) AND valid; // set the mix value
				
				return valid;
			};
			// Materials
			static bool setValue(String name, const Materials::MaterialBase*mat);
			// KeyFrame
			static bool setValue(String name, const Render::Animation::KeyFrame& frame);
			// Matrix4 collection
			static bool setValue(String name, const std::vector<glm::mat4>& matrices);
			// LightBase
			static bool setValue(String name, const Component::LightBase* light);

			static void setActive(const unsigned& shaderId);
			static void cleanUp();
		private:
			static bool setValue(String name, const Materials::Forward& fwd);
			static bool setValue(String name, const Materials::PBR& mat);
			static unsigned active;
		};
	}
}

