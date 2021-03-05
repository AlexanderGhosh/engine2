#version 460 core
layout(location = 0) in vec3 pos;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
};

out VS_out {
    vec3 colour;
    mat4 translation;
} vs_out;

uniform vec3 position;
uniform vec3 colour;

void main() {
    gl_Position = projection * view * vec4(position, 1);
    vs_out.colour = colour;
    vs_out.translation = projection * view;
}