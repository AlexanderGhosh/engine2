#version 460 core
layout(location = 0) in vec3 pos;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
    mat4 lightSpaceMatrix;
};

out VS_out {
    vec3 position;
    vec3 colour;
    vec3 left;
    vec3 right;
} vs_out;

uniform vec3 position;
uniform vec3 colour;
uniform vec3 left;
uniform vec3 right;

void main() {
    gl_Position = projection * view * vec4(position, 1);

    vs_out.position = position;
    vs_out.colour = colour;
    vs_out.left = left;
    vs_out.right = right;
}