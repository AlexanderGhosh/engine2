#version 460 core

out vec4 FragCol;
in vec2 texCoords;

uniform sampler2D tex;

void main()
{
    FragCol = texture(tex, texCoords);
}