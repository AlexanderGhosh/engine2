#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

out vec2 TextureCoords;

void main() {
	gl_Position = vec4(pos, 1);
    TextureCoords = tex;
    TextureCoords.y *= -1;
}