#version 460 core
layout(location = 0) in vec3 pos;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
    mat4 lightSpaceMatrix;
};

out VS_OUT{
    vec3 texCoords;
} vs_out;

void main() {
    vec4 p = projection * mat4(mat3(view)) * vec4(pos, 1);
    gl_Position = p.xyww;
    vs_out.texCoords = pos;
}