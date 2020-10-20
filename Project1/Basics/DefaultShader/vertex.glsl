#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

uniform mat4 model;
out vec3 fragPos;

void main() {
    // projection * view * model * 
	gl_Position = projection * view * model * vec4(pos, 1);
    fragPos = pos;
}