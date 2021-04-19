#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
    float camExposure;
};

out float CameraExposure;
out float GammaValue;
out vec2 TextureCoords;
out vec3 cameraPosition;

void main() {
	gl_Position = vec4(pos, 1);
    TextureCoords = tex;
    TextureCoords.y *= -1;
    cameraPosition = viewPosition;
    CameraExposure = camExposure;
    GammaValue = gamma;
}