#version 460 core

struct MatItem4 {
    vec4 raw;
    sampler2D id;
    float mixValue;
    float repeatValue;
};
struct Material{
    MatItem4 albedo;
};

uniform Material material;
in vec2 TexCoords;

float getAlpha(MatItem4 item){
    return mix(item.raw, texture(item.id, TexCoords * item.repeatValue), item.mixValue).a;
}


void main()
{
    if(getAlpha(material.albedo) < 0.5){
        discard;
    }
}