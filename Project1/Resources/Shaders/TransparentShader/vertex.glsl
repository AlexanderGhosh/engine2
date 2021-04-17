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

out vec3 WorldFragmentPosition;
out vec3 CameraPosition;
out vec3 NormalIn;
out vec2 TextureCoords;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1);
    WorldFragmentPosition = vec3(model * vec4(pos, 1));
    TextureCoords = tex;
    NormalIn = norm;
    CameraPosition = viewPosition;
}