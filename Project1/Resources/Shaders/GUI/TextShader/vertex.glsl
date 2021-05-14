#version 460 core
layout(location = 0) in vec4 pos_tex;

layout(std140, binding = 2) uniform Matrices
{
    mat4 projection;
};

uniform mat4 model;

out vec2 TextureCoords;

vec3 map(vec2 p) {
    return vec3((p.xy + 1) * 0.5, 0);
};

void main() {
	gl_Position = projection * model * vec4(map(pos_tex.xy), 1);
    TextureCoords = pos_tex.zw;
}