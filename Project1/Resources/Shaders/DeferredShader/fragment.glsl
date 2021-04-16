#version 460 core

layout (location = 0) out vec4 positionOut;
layout (location = 1) out vec4 albedoOut;
layout (location = 2) out vec4 normalOut;
layout (location = 3) out vec4 MetRouAOOut;

in vec2 TexCoords;
in vec3 WorldPosition;
in vec3 Normal;

struct MatItem4 {
    vec4 raw;
    sampler2D id;
    float mixValue;
};
struct MatItem3 {
    vec3 raw;
    sampler2D id;
    float mixValue;
};
struct MatItem1 {
    float raw;
    sampler2D id;
    float mixValue;
};

struct Material {
    MatItem4 albedo;

    MatItem3 normal;

    MatItem1 metalic;

    MatItem1 roughness;

    MatItem1 ao;
};

uniform Material material;

vec3 getData(MatItem4 item){
    return mix(item.raw, texture(item.id, TexCoords), item.mixValue).rgb;
}
vec3 getData(MatItem3 item){
    return mix(item.raw, texture(item.id, TexCoords).rgb, item.mixValue);
}
float getData(MatItem1 item){
    return mix(item.raw, texture(item.id, TexCoords).r, item.mixValue);
}

void main()
{    
    vec3 albedo     = getData(material.albedo);
    vec3 normal     = getData(material.normal);
    float metalic   = getData(material.metalic);
    float roughness = getData(material.roughness);
    float ao        = getData(material.ao);

    positionOut = vec4(WorldPosition, 1.0);
    albedoOut   = vec4(albedo, 1.0);
    normalOut   = vec4(Normal, 1.0);
    MetRouAOOut = vec4(metalic, roughness, ao, 1.0);
}  