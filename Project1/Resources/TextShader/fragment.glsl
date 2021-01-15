#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform vec3 color;
uniform sampler2D text;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
    FragColor = vec4(color, 1.0) * sampled;
}