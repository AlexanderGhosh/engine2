#version 460 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_out {
    mat4 translation;
} gs_in[];

out vec3 f_col;
uniform vec3 left;
uniform vec3 right;
uniform vec3 colour;

void main()
{
    f_col = colour;

    gl_Position = gs_in[0].translation * (vec4(left, 1));
    EmitVertex();
    gl_Position = gs_in[0].translation * (vec4(right, 1));
    EmitVertex();

    EndPrimitive();
}