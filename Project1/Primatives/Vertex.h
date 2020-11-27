#pragma once
#include <vector>
#include <array>
#include <glm.hpp>
namespace Primative {
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 tex;
		glm::vec3 norm;
		inline Vertex(glm::vec3 p, glm::vec2 t = { 0, 0 }, glm::vec3 n = { 0, 1, 0 }) : pos(p), tex(t), norm(n) { };
	};
}
