#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 normals;
    vec2 texCoords;
    vec3 worldPos;
    vec3 camPos;
} fs_in;

struct Material {
     vec4 albedo_vec;
     sampler2D albedo_id;

     vec4 normal_vec;
     sampler2D normal_id;

     vec4 metalic_vec;
     sampler2D metalic_id;
     
     vec4 roughness_vec;
     sampler2D roughness_id;

     vec4 ao_vec;
     sampler2D ao_id;
};

uniform Material material;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 norm_;
vec3 getNormalFromMap()
{
    vec3 tangentNormal = norm_ * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.worldPos);
    vec3 Q2  = dFdy(fs_in.worldPos);
    vec2 st1 = dFdx(fs_in.texCoords);
    vec2 st2 = dFdy(fs_in.texCoords);

    vec3 N  = normalize(fs_in.normals);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    cosTheta = min(cosTheta, 0);
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
void main()
{		
    vec3 albedo_ = material.albedo_vec.rgb;
    if (material.albedo_vec.a == 0){
        albedo_ = texture(material.albedo_id, fs_in.texCoords).rgb;
    }
    
    norm_ = material.normal_vec.rgb;
    if (material.normal_vec.a == 0){
        norm_ = normalize(texture(material.normal_id, fs_in.texCoords).rgb);
    }
    FragColor = vec4(getNormalFromMap(), 1);
    //return;
    float metallic = material.metalic_vec.r;
    if (material.metalic_vec.a == 0){
        metallic = texture(material.metalic_id, fs_in.texCoords).r;
    }

    float roughness = material.roughness_vec.r;
    if (material.roughness_vec.a == 0){
        roughness = texture(material.roughness_id, fs_in.texCoords).r;
    }

    float ao = material.ao_vec.r;
    if (material.ao_vec.a == 0){
        ao = texture(material.ao_id, fs_in.texCoords).r;
    }

    vec3 albedo = pow(albedo_, vec3(2.2));
    /*float metallic  = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao        = texture(aoMap, TexCoords).r;*/

    vec3 N = getNormalFromMap();
    vec3 V = normalize(fs_in.camPos - fs_in.worldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);


    // reflectance equation
    vec3 Lo = vec3(0.0);
    vec3 lightPositions[1] = {vec3(2, 2 ,0)};
    vec3 lightColors[1] = {vec3(300, 300, 300)};
    for(int i = 0; i < 1; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - fs_in.worldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - fs_in.worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color /= color + vec3(1.0);
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
    FragColor = vec4(color, 1.0);
}