#pragma once
#include <glm.hpp>
#include <array>
#include <GL/glew.h>
namespace Materials {
	class MatItem {
	private:
		glm::vec3 raw;
		int texId;
	public:
		inline MatItem() : raw(0), texId(-1) { };
		inline MatItem(glm::vec3 raw) : MatItem() { this->raw = raw; };
		inline MatItem(unsigned id) : MatItem() { texId = id; };
		inline const glm::vec3& getRaw() const { return raw; };
		inline const int& getId() const { return texId; };

		inline void setRaw(const glm::vec3& raw) { this->raw = raw; texId = -1; };
		inline void setId(const int& id) { this->texId = id; };
		inline const bool hasTex() const { return texId >= 0; };

		void operator()(const glm::vec3& raw);
		void operator()(const unsigned& id);
	};
	struct Forward {
	private:
		std::array<MatItem, 2> diff_spec;
	public:
		float shininess;
		inline MatItem& getDiffuse() { return diff_spec[0]; };
		inline MatItem& getSpecular() { return diff_spec[1]; };
		inline std::array<MatItem, 2>& getDiffSpec() { return diff_spec; };
		void activateTextures() const;
	};

	struct PBR {

	};
};


