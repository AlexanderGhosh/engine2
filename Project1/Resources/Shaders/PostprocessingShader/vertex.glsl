#version 460 core
layout(location = 0) in vec3 tex;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
    mat4 lightSpaceMatrix;
};

out vec2 tex_coords;

void main(){
    vec3 pos = tex;
    pos *= 2;
    pos -= 1;
    gl_Position = vec4(pos * 0.95, 1);
    tex_coords = tex.xy;
}
