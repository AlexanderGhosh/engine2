#version 460 core
layout(location = 0) in vec3 pos;

layout(std140, binding = 1) uniform Matrices
{
    // mat4 view;
    // mat4 projection;
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
};
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1);
}