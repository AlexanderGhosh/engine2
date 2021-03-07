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
layout(std140, binding = 1) uniform LSMatrices
{
    mat4 lsMatrix;
};

out VS_OUT{
    vec3 normals;
    vec2 texCoords;
    vec3 worldPos;
    vec3 camPos;
    vec4 ws;
    vec4 lsPos;
} vs_out;

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
    //p.y = height;
    texCoords = tex;
	gl_Position = projection * view * model * vec4(p, 1);
    vs_out.worldPos = vec3(model * vec4(p, 1));
    vs_out.normals = mat3(model) * vec3(0, 1, 0);
    vs_out.camPos = viewPosition;
    vs_out.texCoords = tex;
    vs_out.lsPos = lsMatrix * vec4(vs_out.worldPos, 1);
}