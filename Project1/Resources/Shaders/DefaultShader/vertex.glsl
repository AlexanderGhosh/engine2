#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
    mat4 lightSpaceMatrix;
};

out VS_OUT{
    vec3 fragPos;
    vec3 normals;
    vec2 texCoords;
    vec3 viewPos;
    float gammaValue;
    vec4 fragPosLightSpace;
} vs_out;

uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1);

    vs_out.fragPos = pos;
    vs_out.texCoords = tex;
    vs_out.normals = norm;
    vs_out.viewPos = viewPosition;
    vs_out.gammaValue = gamma;
    vs_out.fragPosLightSpace = lightSpaceMatrix * vec4(pos, 1.0);
}