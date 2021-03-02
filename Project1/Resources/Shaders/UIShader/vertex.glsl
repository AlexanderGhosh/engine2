#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

layout(std140, binding = 1) uniform Matrices
{
    mat4 projection;
};

out vec2 texCoords;
uniform mat4 model;

void main() {
	gl_Position = projection * model * vec4(pos, 1);
    texCoords = tex;
}