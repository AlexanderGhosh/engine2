#pragma once
#include <vector>
#include <array>
#include <glm.hpp>
#include "Vertex.h"
namespace Primative {
	struct Mesh
	{
		std::vector<Vertex> verts;
		std::vector<unsigned> indices;
		const Vertex operator[](int index) const;
		inline Mesh(std::vector<Vertex> verts = { }, std::vector<unsigned> indi = { }) : verts(verts), indices(indi) { };
	};
}

