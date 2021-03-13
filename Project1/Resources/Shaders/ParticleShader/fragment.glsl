#version 460 core

out vec4 FragCol;

in vec2 textCoords;
uniform vec4 col;
uniform float mix_val;
uniform sampler2D tex;


void main()
{
    vec4 colour = mix(texture(tex, textCoords), col, mix_val);
    FragCol = vec4(colour);
}