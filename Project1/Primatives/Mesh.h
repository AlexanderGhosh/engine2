#pragma once
#include <vector>
#include <array>
#include <glm.hpp>
#include <string>
#include "Vertex.h"

namespace Primative {
	struct Vertex;
	struct Mesh
	{
		std::vector<Vertex> verts;
		std::vector<unsigned> indices;
		std::string name;
		const Vertex operator[](int index) const;
		Mesh(std::vector<Vertex> verts = { }, std::vector<unsigned> indi = { }, std::string name = "");
	};
}

