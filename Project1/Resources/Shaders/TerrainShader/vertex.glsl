#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
};

out vec2 texCoords;
out float height;
uniform mat4 model;
uniform sampler2D hm;

float map(float val, float low, float high, float l, float h){
    return l + ((h - l) / (high - low)) * (val - low);
}
void main() {
    vec3 p = pos;
    height = map(texture(hm, tex).r, 0, 1, -0.5, 0.5);
    p.y = height;
    texCoords = tex;
	gl_Position = projection * view * model * vec4(p, 1);
}