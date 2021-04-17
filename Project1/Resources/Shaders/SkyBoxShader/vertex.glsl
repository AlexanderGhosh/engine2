#version 460 core
layout(location = 0) in vec3 pos;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
};

out vec3 texCoords;

void main() {
    vec4 p = projection * mat4(mat3(view)) * vec4(pos, 1.0);
    gl_Position = p.xyww;
    texCoords = pos;
}