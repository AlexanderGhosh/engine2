#version 460 core
layout(location = 0) in vec4 pos_tex;

layout(std140, binding = 1) uniform Matrices
{
    mat4 projection;
};

out vec2 texCoords;

void main() {
	gl_Position = projection * vec4(pos_tex.xy, 0, 1);
    texCoords = pos_tex.zw;
}