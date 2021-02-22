#pragma once
#include <array>
#include <glm.hpp>
#include "../Rendering/Animation/Bones.h"
#include "../Utils/General.h"

namespace Primative {
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 tex;
		glm::vec3 norm;

		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];

		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
		inline Vertex(glm::vec3 p, glm::vec2 t = { 0, 0 }, glm::vec3 n = { 0, 1, 0 }) : pos(p), tex(t), norm(n), ids(-1), weights(0) { };
	};
}
