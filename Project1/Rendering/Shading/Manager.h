#pragma once
#include <GL/glew.h>
#include <string>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "../../Primatives/Material.h"
namespace Render {
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
			static bool setValue(const std::string& name, const Materials::MatItem& fwd, short& texUnit);
			static bool setValue(const std::string& name, Materials::Forward& fwd);
			static bool setValue(const std::string& name, Materials::PBR& mat);
			static void setActive(const unsigned& shaderId);
			static void cleanUp();
		private:
			static unsigned active;
		};
	}
}

