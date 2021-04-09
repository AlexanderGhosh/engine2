#version 460 core

out vec4 FragColor;

in vec2 texCoords;

struct MatItem3 {
    vec3 raw;
    sampler2D id;
    float mixValue;
};
uniform MatItem3 col;

void main() {
    FragColor = vec4(mix(col.raw, texture(col.id, texCoords).rgb, col.mixValue), 1);
}