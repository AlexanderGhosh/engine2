#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 0) in vec2 tex;

out VS_OUT {
    vec2 texCoords;
} vs_out;

void main() {
	gl_Position = vec4(pos, 1);
    vs_out.texCoords = tex;
}