#version 460 core
layout(location = 0) in vec3 pos;

layout(std140, binding = 1) uniform Matrices
{
    mat4 lsMatrix;
};
uniform mat4 model;

void main() {
	gl_Position = lsMatrix * model * vec4(pos, 1);
}