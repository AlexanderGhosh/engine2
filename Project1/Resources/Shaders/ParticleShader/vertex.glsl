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
uniform vec3 center[200];
uniform vec3 scale[200];

out vec2 textCoords;

void main() {

    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 up = vec3(view[0][1], view[1][1], view[2][1]);
    vec3 s = scale[gl_InstanceID];
    vec3 p =
    center[gl_InstanceID]
    + right * pos.x * s.x
    + up * pos.y * s.y;
    
	gl_Position = projection * view * vec4(p, 1);
    textCoords = tex;
}