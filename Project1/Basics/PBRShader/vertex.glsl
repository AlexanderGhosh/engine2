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
    vec3 normals;
    vec2 texCoords;
    vec3 worldPos;
    vec3 camPos;
} vs_out;

uniform mat4 model;

void main() {
    vs_out.worldPos = vec3(model * vec4(pos, 1));
	
    gl_Position = projection * view * model * vec4(pos, 1);

    vs_out.normals = mat3(model) * norm;

    vs_out.camPos = viewPosition;
    vs_out.texCoords = tex;
}