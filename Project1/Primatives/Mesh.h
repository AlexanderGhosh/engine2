#pragma once
#include <vector>
#include <array>
#include <glm.hpp>

namespace Primative {
	struct Vertex;
	struct Mesh
	{
		std::vector<Vertex> verts;
		std::vector<unsigned> indices;
		const Vertex operator[](int index) const;
		Mesh(std::vector<Vertex> verts = { }, std::vector<unsigned> indi = { });
	};
}

