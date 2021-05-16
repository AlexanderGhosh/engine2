#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

layout(std140, binding = 2) uniform Matrices
{
    mat4 projection;
};

out vec2 TextureCoords;
out vec2 FragPos;
out vec2 FragPosTranslated;
out vec2 Dimentions;

uniform vec2 dimentions;
uniform mat4 model;

vec2 map(vec3 p) {
    return (p.xy + 1) * 0.5;
};

void main() {
    FragPos = map(pos) * dimentions;
    FragPosTranslated = (model * vec4(map(pos), 0, 1)).xy;
	gl_Position = projection * model * vec4(map(pos), 0, 1);
    TextureCoords = tex;
    Dimentions = dimentions;
}