#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

/*layout(std140, binding = 1) uniform Matrices
{
    mat4 projection;
    mat4 view;
};*/

uniform mat4 model;

void main() {
    // projection * view * model * 
	gl_Position = vec4(pos, 1);
}