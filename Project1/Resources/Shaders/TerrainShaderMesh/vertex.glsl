#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
};

out vec3 norm;
out vec2 texCoords;
out float height;
uniform mat4 model;

void main() {
    height = pos.y + 0.5;
    texCoords = tex;
	gl_Position = projection * view * model * vec4(pos, 1);
    norm = normal;
}