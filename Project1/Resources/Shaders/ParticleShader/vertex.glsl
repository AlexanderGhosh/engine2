#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
    float exposure;
};
uniform vec3 positions[200];
// uniform vec3 scale[200];

out vec2 TextureCoords;
out float GammaValue;
out float CameraExposure;

void main() {

    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 up = vec3(view[0][1], view[1][1], view[2][1]);
    vec3 s = vec3(1);
    vec3 p = positions[gl_InstanceID] + right * pos.x * s.x + up * pos.y * s.y;
    
	gl_Position = projection * view * vec4(p, 1);
    TextureCoords = tex;
    GammaValue = gamma;
    CameraExposure = exposure;
}