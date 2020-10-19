#pragma once
#include <vector>
#include <array>
#include <glm.hpp>
using namespace std;
using namespace glm;
namespace Primative {
	struct Vertex
	{
		vec3 pos;
		vec2 tex;
		vec3 norm;
		inline Vertex(vec3 p, vec2 t = { 0, 0 }, vec3 n = { 0, 1, 0 }) : pos(p), tex(t), norm(n) { };
	};
}
