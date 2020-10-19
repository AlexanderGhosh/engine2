#pragma once
#include <vector>
#include <array>
#include <glm.hpp>
#include "Vertex.h"
using namespace std;
using namespace glm;
namespace Primative {
	struct Mesh
	{
		vector<Vertex> verts;
		vector<unsigned> indices;
		const Vertex operator[](int index) const;
	};
}

