#version 460 core

out vec4 FragCol;

layout(std140, binding = 1) uniform Colors
{
    vec3 col;
};

void main() {
    FragCol = vec4(col, 1);
}