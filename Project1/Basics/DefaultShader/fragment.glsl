#version 460 core

out vec4 FragCol;
in vec3 fragPos;
in vec2 texCoords;

uniform sampler2D tex;

void main() {
     FragCol = vec4(1, 0, 0, 1);;
}