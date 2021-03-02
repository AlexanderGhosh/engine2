#version 460 core
layout (points) in;
layout (points, max_vertices = 1) out;

in VS_out {
    vec3 position;
    vec3 colour;
} gs_in[];

out vec3 f_col;

void main()
{
    f_col = gs_in[0].colour;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}