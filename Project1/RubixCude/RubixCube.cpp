#include "RubixCube.h"

Cube::Cube() : relPos(0), colours(), faces(), type(Null)
{
}

Cube::Cube(Type type, glm::ivec3 relPos, std::vector<glm::vec3> colours) : Cube()
{
	this->type = type;
	this->relPos = relPos;
	this->colours = colours;
}

Cube::Cube(std::vector<glm::vec3> colours, std::vector<char> faces) : Cube()
{
	this->colours = colours;
	this->faces = faces;
}

RubixCube::RubixCube() : cubes()
{
	CreateSolved();
}

void RubixCube::CreateSolved()
{
	for (int i = 0; i < centerIndices.size(); i++) {
		const glm::ivec3& centerI = centerIndices[i];
		const glm::vec3& col = colours[i];
		GetCube(centerI) = Cube(Cube::Center, centerI, { col });
	}
}

Cube& RubixCube::GetCube(glm::ivec3 position)
{
	cubes[position.x][position.y][position.z];
}

CubeSlice& RubixCube::GetColumn(int col)
{
	return cubes[0];
}

CubeSlice& RubixCube::GetRow(int row)
{
	return cubes[0];
}
