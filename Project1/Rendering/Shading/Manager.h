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
namespace Render {
	namespace Animation {
		struct KeyFrame;
	}
	namespace Shading {
		class Manager
		{
		public:
			static bool setValue(const std::string& name, int val);
			static bool setValue(const std::string& name, float val);
			static bool setValue(const std::string& name, glm::vec2 val);
			static bool setValue(const std::string& name, glm::vec3 val);
			static bool setValue(const std::string& name, glm::vec4 val);
			static bool setValue(const std::string& name, glm::mat3 val);
			static bool setValue(const std::string& name, glm::mat4 val);
			template<class T>
			static inline bool setValue(const std::string& name, const Materials::MatItemBase<T>& item, unsigned& texUnit) {
				bool valid = true;
				valid = Render::Shading::Manager::setValue(name + ".id", static_cast<int>(item.getCurrentTexId())) AND valid; // set to texture unit
				valid = Render::Shading::Manager::setValue(name + ".raw", item.getCurrentRaw()) AND valid; // set the raw value
				valid = Render::Shading::Manager::setValue(name + ".mixValue", item.getCurrentMixValue()) AND valid; // set the mix value
				
				return valid;
			};
			static bool setValue(const std::string& name, const Materials::MaterialBase*mat);
			static bool setValue(const std::string& name, const Render::Animation::KeyFrame& frame);
			static bool setValue(const std::string& name, const std::vector<glm::mat4>& matrices);
			static void setActive(const unsigned& shaderId);
			static void cleanUp();
		private:
			static bool setValue(const std::string& name, const Materials::Forward& fwd);
			static bool setValue(const std::string& name, const Materials::PBR& mat);
			static unsigned active;
		};
	}
}

