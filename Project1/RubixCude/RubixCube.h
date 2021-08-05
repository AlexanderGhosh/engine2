#pragma once
#include <vector>
#include <array>
#include <glm.hpp>

class Cube;

template<class T, int x>
using Array1D = std::array<T, x>;
template<class T, int x, int y>
using Array2D = std::array<std::array<T, x>, y>;
template<class T, int x, int y, int z>
using Array3D = std::array<std::array<std::array<T, x>, y>, z>;

using CubeArray = Array3D<Cube, 3, 3, 3>;
using CubeSlice = Array2D<Cube, 3, 3>;

class Cube {
public:
	enum Type {
		Center, Corner, Side, Null
	};
private:
	glm::vec3 relPos;
	std::vector<glm::vec3> colours;
	std::vector<char> faces;
	Type type;
public:
	Cube();
	Cube(Type type, glm::ivec3 relPos, std::vector<glm::vec3> colours);
	Cube(std::vector<glm::vec3> colours, std::vector<char> faces);
	~Cube() = default;
};

class RubixCube
{
	CubeArray cubes;
	// top    - White
	// bottom - Yellow
	// left   - Green
	// right  - Blue
	// front  - Red
	// back   - Orange
	const Array1D<glm::ivec3, 6> centerIndices = {
		glm::ivec3(1, 0, 1),
		glm::ivec3(1, 2, 1),
		glm::ivec3(0, 1, 1),
		glm::ivec3(2, 1, 1),
		glm::ivec3(1, 1, 0),
		glm::ivec3(1, 1, 2),
	};

	const Array1D<glm::vec3, 6> colours = {
		glm::vec3(1),
		glm::vec3(1, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0.64, 0),
	};
public:
	RubixCube();
	~RubixCube() = default;

	void CreateSolved();

	Cube& GetCube(glm::ivec3 position);
	CubeSlice& GetColumn(int col);
	CubeSlice& GetRow(int row);
};

