#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 LSMatrix;
uniform mat4 model;

out vec2 TexCoords;

void main() {
	gl_Position = LSMatrix * model * vec4(pos, 1);
    TexCoords = tex;
}