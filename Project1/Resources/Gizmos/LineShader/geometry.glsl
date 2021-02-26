#version 460 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_out {
    vec3 position;
    vec3 colour;
    vec3 left;
    vec3 right;
} gs_in[];

out vec3 f_col;

void main()
{
    f_col = gs_in[0].colour;
    gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].left, 1);
    EmitVertex();
    f_col = gs_in[0].colour;
    gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].right, 1);
    EmitVertex();
    EndPrimitive();
}