#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec2 texCoords;
} fs_in;

uniform sampler2D depthMap;
void main() {
    float depthValue = texture(depthMap, fs_in.texCoords).r;
    FragColor = vec4(vec3(depthValue), 1);
}