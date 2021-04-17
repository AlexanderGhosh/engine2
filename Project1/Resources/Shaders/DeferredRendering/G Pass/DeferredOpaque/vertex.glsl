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
};
uniform mat4 model;

out vec2 TexCoords;
out vec3 WorldPosition;
out vec3 Normal;

void main() {
    WorldPosition = vec3(model * vec4(pos, 1));
	gl_Position = projection * view * model * vec4(pos, 1);
    TexCoords = tex;
    Normal = norm;
}