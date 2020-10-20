#pragma once
#include <GL/glew.h>
#include <string>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
namespace Render {
	namespace Shading {
		class Manager
		{
		public:
			static bool setValue(std::string name, int val) {
				int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
				if (loc < 0) return false;
				glUniform1i(loc, val);
				return true;
			};

			static bool setValue(std::string name, float val) {
				int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
				if (loc < 0) return false;
				glUniform1f(loc, val);
				return true;
			};
			static bool setValue(std::string name, glm::vec2 val) {
				int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
				if (loc < 0) return false;
				glUniform2f(loc, val[0], val[1]);
				return true;
			};
			static bool setValue(std::string name, glm::vec3 val) {
				int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
				if (loc < 0) return false;
				glUniform3f(loc, val[0], val[1], val[2]);
				return true;
			};
			static bool setValue(std::string name, glm::vec4 val) {
				int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
				if (loc < 0) return false;
				glUniform4f(loc, val[0], val[1], val[2], val[3]);
				return true;
			};
			static bool setValue(std::string name, glm::mat3 val) {
				int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
				if (loc < 0) return false;
				glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(val));
				return true;
			};
			static bool setValue(std::string name, glm::mat4 val) {
				int loc = glGetUniformLocation(Render::Shading::Manager::active, name.c_str());
				if (loc < 0) return false;
				glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
				return true;
			};
			static inline void setActive(const unsigned& shaderId) { Manager::active = shaderId; };
		private:
			static unsigned active;
		};
	}
}

