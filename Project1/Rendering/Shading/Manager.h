#pragma once
#include <GL/glew.h>
#include <string>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
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
			static bool setValue(const std::string& name, int val);
			static bool setValue(const std::string& name, float val);
			static bool setValue(const std::string& name, glm::vec2 val);
			static bool setValue(const std::string& name, glm::vec3 val);
			static bool setValue(const std::string& name, glm::vec4 val);
			static bool setValue(const std::string& name, glm::mat3 val);
			static bool setValue(const std::string& name, glm::mat4 val);
			static bool setValue(const std::string& name, const Materials::MatItem& fwd, short& texUnit);
			static bool setValue(const std::string& name, const Materials::Material*mat);
			static bool setValue(const std::string& name, const Render::Animation::KeyFrame& frame);
			static void setActive(const unsigned& shaderId);
			static void cleanUp();
		private:
			static bool setValue(const std::string& name, const Materials::Forward& fwd);
			static bool setValue(const std::string& name, const Materials::PBR& mat);
			static unsigned active;
		};
	}
}

