#version 460 core
layout(location = 0) in vec3 pos;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
};
uniform mat4 model;

void main() {
	gl_Position = vec4(pos, 1);
}