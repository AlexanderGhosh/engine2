#version 460 core

out vec4 FragColour;

in vec2 TextureCoords;
in float GammaValue;
in float CameraExposure;

struct MatItem4 {
    vec4 raw;
    sampler2D id;
    float mixValue;
    float repeatValue;
};

uniform MatItem4 albedo;

vec4 getData(MatItem4 item){
    return mix(item.raw, texture(item.id, TextureCoords * item.repeatValue), item.mixValue);
}
float getAlpha(MatItem4 item){
    return mix(item.raw, texture(item.id, TextureCoords * item.repeatValue), item.mixValue).a;
}

void main()
{
    vec3 albedoCol = getData(albedo).rgb;
    albedoCol = vec3(0, 0, 1);

    /*albedoCol = pow(albedoCol, -vec3(1.0 / GammaValue));

    albedoCol = log(vec3(1.0) - albedoCol) / -CameraExposure;*/

    FragColour = vec4(albedoCol, getAlpha(albedo));

    /*
    return vec3(1.0) - exp(-a * CameraExposure); // more controll
    return a / (a + vec3(1.0)); // less controll
    */
}