#pragma once
#include <vector>
#include <glm.hpp>
#include "../Rendering/Animation/Bones.h"

namespace Primative {
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 tex;
		glm::vec3 norm;
		std::vector<Render::Animation::BoneDetails> boneDetails;
		inline Vertex(glm::vec3 p, glm::vec2 t = { 0, 0 }, glm::vec3 n = { 0, 1, 0 }) : pos(p), tex(t), norm(n), boneDetails() { };
	};
}
