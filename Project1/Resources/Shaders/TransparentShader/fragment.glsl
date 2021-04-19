#version 460 core
precision highp float;

out vec4 FragColour;

// constants
const float PI = 3.14159265359;
const int maxPointLights = 100;
const int maxDirectionalLights = 5;
const int maxSpotLights = 100;

void SpecularTerm(float dotNH, float a2, float cosTheta, vec3 F0, float dotNV, 
                float dotNL, float Roughness, inout vec3 info[2]);
vec3 GammaCorrect(vec3 col);


// PBR Functions
float Distribution(float dotNH, float a2);
vec3 Reflectance(float cosTheta, vec3 F0);
float Geometry(float dotNV, float dotNL, float Roughness);
float CorrectionFactor(float dotNV, float dotNL);


//Material Data Structures
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

// Material Functions
vec3 getData(MatItem4 item);
vec3 getData(MatItem3 item);
vec3 getNormal(MatItem3 item, vec3 normal);
float getData(MatItem1 item);
float getAlpha(MatItem4 item);

// Light Source Data Structures
struct PointLight {
    vec3 position;
    vec3 colour;
    float brightness;
};
uniform int numberOfPointLights;
uniform PointLight pointLights[maxPointLights];

struct DirectionalLight {
    vec3 direction;
    vec3 colour;
    float brightness;
};
uniform int numberOfDirectionalLights;
uniform DirectionalLight directionalLights[maxDirectionalLights];

// Light Source Functions
// POINT LIGHTS
vec3 ProcessPointLight(PointLight light, vec3 WFP, vec3 VD, vec3 N, vec3 RS, vec3 AD, float A2, float R, float M, float dotNV, float alphaValue);
vec3 PointLights(PointLight pointLights[maxPointLights], int numberOfPointLights, vec3 WFP, vec3 VD, vec3 N, vec3 RS, vec3 AD, float A2, float R, float M, float dotNV, float alphaValue);
// DIRECTIONAL LIGHTS
vec3 ProcessDirectionalLight(DirectionalLight light);
vec3 DirectionalLights(DirectionalLight directionalLights[maxDirectionalLights]);
// SPOT LIGHTS

uniform Material material;

in vec3 WorldFragmentPosition;
in vec3 CameraPosition;
in vec3 NormalIn;
in vec2 TextureCoords;

void main() {
    // vec3  = texture(positionTex, TextureCoords).xyz;
    vec3 Albedo     = pow(getData(material.albedo), vec3(2.2)); // PBR gamma corrects
    vec3 Normal     = normalize(getNormal(material.normal, NormalIn));
    Normal = normalize(NormalIn);

    float Metallic  = getData(material.metalic);
    Metallic = 0.5;
    float Roughness = getData(material.roughness);
    float AO        = getData(material.ao);


    // constants
    vec3 viewDirection = normalize(CameraPosition - WorldFragmentPosition);
    float dotNV = max(dot(Normal, viewDirection), 0.0);
    float alpha = Roughness * Roughness;
    float alpha2 = max(0.001, alpha * alpha);
    float alphaValue = getAlpha(material.albedo);
    vec3 albedoDiffuse = (Albedo * alphaValue) / PI;
    
    // surface reflection at when looking from 0 degrees
    vec3 realSpecular = mix(vec3(0.04), Albedo, Metallic); // 0.04 assumption for metals


    vec3 accumlativeLight = PointLights(pointLights, numberOfPointLights, WorldFragmentPosition, viewDirection, Normal, realSpecular, albedoDiffuse, alpha2, Roughness, Metallic, dotNV, alphaValue);


    vec3 ambient = vec3(0.03) * Albedo * AO;
    vec3 colour = ambient + accumlativeLight;


    colour /= colour + vec3(1.0); // HDR
    colour = GammaCorrect(colour);
    
    //colour = Albedo;

    FragColour = vec4(colour, alphaValue);
};

vec3 ProcessPointLight(PointLight light, vec3 WFP, vec3 VD, vec3 N, vec3 RS, vec3 AD, float A2, float R, float M, float dotNV, float alphaValue){
    // constants
    vec3 lightDirection = normalize(light.position - WFP);
    vec3 H = normalize(VD + lightDirection);
    float dotNL = max(dot(N, lightDirection), 0.0);
    float dotNH = max(dot(N, H), 0.0);
    float dotVH = max(dot(VD, H), 0.0);

    // light radiance
    float dist = length(light.position - WFP);
    float attenuation = 1.0 / pow(dist, 2.0);
    vec3 radiance = light.colour * attenuation;

    // functions
    vec3 specularData[2];
    SpecularTerm(dotNH, A2, dotVH, RS, dotNV, 
                                    dotNL, R, specularData);

    vec3 specular = specularData[0];
    vec3 ks = specularData[1];
    vec3 kd = vec3(1.0) - ks;  
    kd *= 1.0 - M;

    return (kd * AD + specular) * radiance * dotNL * light.brightness;
}
vec3 PointLights(PointLight pointLights[maxPointLights], int numberOfPointLights, vec3 WFP, vec3 VD, vec3 N, vec3 RS, vec3 AD, float A2, float R, float M, float dotNV, float alphaValue) {
    vec3 accumlativeLight = vec3(0);
    for(int i = 0; i < numberOfPointLights; i++){
        accumlativeLight += ProcessPointLight(pointLights[i], WFP, VD, N, RS, AD, A2, R, M, dotNV, alphaValue);
    }
    return accumlativeLight;
}


vec3 GammaCorrect(vec3 col) {
    return pow(col, vec3(1.0/2.2));
};

float Distribution(float dotNH, float a2) {
    // Micro Geometry Distribution Function
    // GGX/Trowbridge approximation
    float denominator = pow(dotNH, 2.0) * (a2 - 1.0) + 1.0;
    denominator = pow(denominator, 2.0);
    denominator *= PI;
    return a2 / max(denominator, 0.0000001);
};

vec3 Reflectance(float cosTheta, vec3 F0) {
    // Fresnel Reflectance Function
    // Schlick approximation
    return F0 + (1.0 - F0) * pow(max(1 - cosTheta, 0.0), 5.0);
};

float Geometry(float dotNV, float dotNL, float Roughness) {
    // Geometry Function
    // Schlick approximation
    float r = Roughness + 1.0;
    float k = (r * r) * 0.125; // div by 8
    float g1 = dotNV / (dotNV * (1.0 - k) + k);
    float g2 = dotNL / (dotNL * (1.0 - k) + k);
    return g1 * g2;
};

float CorrectionFactor(float dotNV, float dotNL) {
    return max(4.0 * dotNV * dotNL, 0.001);
};

void SpecularTerm(float dotNH, float a2, float cosTheta, vec3 F0, float dotNV, 
                float dotNL, float Roughness, inout vec3 info[2]) {
    float D = Distribution(dotNH, a2);
    float G = Geometry(dotNV, dotNL, Roughness);
    vec3 F = Reflectance(cosTheta, F0);
    vec3 numerator = D * G * F;
    info[0] = numerator / CorrectionFactor(dotNV, dotNL);
    info[1] = F;
};

vec3 getData(MatItem4 item) {
    return mix(item.raw, texture(item.id, TextureCoords), vec4(item.mixValue)).rgb;
}
vec3 getData(MatItem3 item) {
    return mix(item.raw, texture(item.id, TextureCoords).rgb, vec3(item.mixValue));
}
vec3 getNormal(MatItem3 item, vec3 normal) {
    return mix(normal, texture(item.id, TextureCoords).rgb, vec3(item.mixValue));
}
float getData(MatItem1 item) {
    return mix(item.raw, texture(item.id, TextureCoords).r, item.mixValue);
}
float getAlpha(MatItem4 item) {
    return mix(item.raw, texture(item.id, TextureCoords), vec4(item.mixValue)).a;
}