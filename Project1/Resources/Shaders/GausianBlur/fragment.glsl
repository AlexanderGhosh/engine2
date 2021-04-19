#version 460 core

out vec4 FragColour;
in vec2 TextureCoords;

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

uniform sampler2D tex;

void main()
{
    vec2 tex_offset = 1.0 / textureSize(tex, 0); // gets size of single texel
    vec3 result = texture(tex, TextureCoords).rgb * weight[0]; // current fragment's contribution
    /*if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(tex, TextureCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(tex, TextureCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(tex, TextureCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(tex, TextureCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }*/
    for(int i = 0; i < 5; i++){
        vec2 off = vec2(0.0, tex_offset.y * i);
        if(horizontal){
            off = vec2(tex_offset.x * i, 0.0);
        }
        result += texture(tex, TextureCoords + off).rgb * weight[i];
        result += texture(tex, TextureCoords - off).rgb * weight[i];
    }
    FragColour = vec4(result, 1);
    //FragColour = vec4(texture(tex, TextureCoords).rgb, 1.0);
}