#include "Rendering.h"

Render::Rendering::Rendering()
{
}

void Render::Rendering::draw(unsigned shaderId)
{
	glUseProgram(shaderId);
	for (const Primative::Buffers& buffer : buffers) {
		buffer.bind();
		buffer.draw();
	}
}

const void Render::Rendering::addMesh(Primative::Mesh* m)
{
	buffers.emplace_back(*m);
};
