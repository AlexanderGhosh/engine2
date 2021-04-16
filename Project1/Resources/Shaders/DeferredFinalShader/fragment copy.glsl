#version 460 core
out vec4 FragColour;

const float PI = 3.14159265359;

vec3 saturate(vec3 value);
float saturate(float value);
void SpecularTerm(float dotNH, float a2, float cosTheta, vec3 F0, float dotNV, 
                float dotNL, float Roughness, inout vec3 info[2]);
vec3 GammaCorrect(vec3 col);

float Distribution(float dotNH, float a2);
vec3 Reflectance(float cosTheta, vec3 F0);
float Geometry(float dotNV, float dotNL, float Roughness);
float CorrectionFactor(float dotNV, float dotNL);

struct PointLight {
    vec3 position;
    vec3 colour;
};
const int numberOfPoints = 100;
uniform PointLight pointLights[numberOfPoints];

uniform sampler2D positionTex;
uniform sampler2D albedoTex;
uniform sampler2D normalTex;
uniform sampler2D MetRouAOTex;

in vec2 TextureCoords;
in vec3 cameraPosition;

void main() {
    vec3 WorldFragmentPosition = texture(positionTex, TextureCoords).xyz;
    vec3 AlbedoRaw             = texture(albedoTex, TextureCoords).xyz; // PBR gamma corrects
    vec3 Normal                = texture(normalTex, TextureCoords).xyz;

    vec3 metRouAO              = texture(MetRouAOTex, TextureCoords).xyz;

    float Metallic             = metRouAO.x;
    float Roughness            = metRouAO.y;
    float AO                   = metRouAO.z;



    // constants
    vec3 viewDirection = normalize(cameraPosition - WorldFragmentPosition);
    float dotNV = max(dot(Normal, viewDirection), 0.0);
    float alpha = Roughness * Roughness;
    float alpha2 = max(0.001, alpha * alpha);
    vec3 albedoDiffuse = AlbedoRaw / PI;

    viewDirection = vec3(0.5, 0, 5);
    /*FragColour = vec4(0.5, 0, 5, 1.0);
    return;*/
    
    // surface reflection at when looking from 0 degrees
    vec3 realSpecular = mix(vec3(0.04), AlbedoRaw, Metallic); // 0.04 assumption for metals

    vec3 accumlativeLight = vec3(0);
    // Spot Light
    for(int i = 0; i < 1; i++){
        PointLight light = pointLights[i];
        // constants
        vec3 lightDirection = normalize(vec3(1, 5, 0) - WorldFragmentPosition);
        vec3 H = normalize(viewDirection + lightDirection);
        float dotNL = max(dot(vec3(0, 1, 0), lightDirection), 0.0);
        float dotNH = max(dot(Normal, H), 0.0);
        float dotVH = max(dot(viewDirection, H), 0.0);

        // light radiance
        float dist = length(light.position - WorldFragmentPosition);
        float attenuation = 1.0 / pow(dist, 2.0);
        vec3 radiance = light.colour * attenuation;

        // functions
        vec3 specularData[2];
        SpecularTerm(dotNH, alpha2, dotVH, realSpecular, dotNV, 
                                        dotNL, Roughness, specularData);

        vec3 specular = specularData[0];
        vec3 ks = specularData[1];
        vec3 kd = vec3(1.0) - ks;  
        kd *= 1.0 - Metallic;

        accumlativeLight += (kd * albedoDiffuse + specular) * radiance * dotNL;
        accumlativeLight = vec3(dotNL);
        accumlativeLight = lightDirection;
    }

    vec3 ambient = vec3(0.03) * AlbedoRaw * AO;
    vec3 colour = ambient + accumlativeLight;

    colour = colour / (colour + vec3(1.0)); // HDR
    // colour = GammaCorrect(colour);

    

    FragColour = vec4(WorldFragmentPosition, 1.0);
};

vec3 saturate(vec3 value) {
    return value;
    // return clamp(value, 0.0, 1.0);`
};
float saturate(float value) {
    return value;
    // return clamp(value, 0.0, 1.0);
};

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
    return F0 + (1 - F0) * pow(max(1 - cosTheta, 0.0), 5.0);
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
    vec3 F = Reflectance(clamp(cosTheta, 0.0, 1.0), F0);
    vec3 numerator = D * G * F;
    info[0] = numerator / CorrectionFactor(dotNV, dotNL);
    info[1] = F;
};