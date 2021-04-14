#version 460 core
precision highp float;

out vec4 FragColor;

in VS_OUT {
    vec3 normals;
    vec2 texCoords;
    vec3 worldPos;
    vec3 camPos;
    vec4 ws;
    vec4 lsPos;
} fs_in;

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

struct PointLight{
    
}

uniform Material material;
uniform samplerCube hdrMap;
uniform samplerCube lbrMap;
uniform sampler2D brdfLUT;
uniform sampler2D shadowMap;

const float PI = 3.14159265359f;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.

vec4 sampleTex(sampler2D tex) {
    return texture(tex, fs_in.texCoords);
}

vec4 getData(MatItem4 item){
    return mix(item.raw, sampleTex(item.id), item.mixValue);
    /*if (item.mixValue == 0){
        return texture(id, fs_in.texCoords).rgb;
    }
    return col.rgb;*/
}
vec3 getData(MatItem3 item){
    return mix(item.raw, sampleTex(item.id).rgb, 1.0 - item.mixValue);
    /*if (item.mixValue == 0){
        return texture(id, fs_in.texCoords).rgb;
    }
    return col.rgb;*/
}
float getData(MatItem1 item){
    return mix(item.raw, sampleTex(item.id).r, 1.0 - item.mixValue);
    /*if (col.a == 0){
        return texture(id, fs_in.texCoords).rgb;
    }
    return col.rgb;*/
}
float getAlpha(MatItem4 item){
    return mix(item.raw.a, sampleTex(item.id).a, 1.0 - item.mixValue);
    /*if(col.a == 0){
        return texture(id, fs_in.texCoords).a;
    }
    return col.a;*/
}
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}
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
    float a2 = roughness * roughness * roughness * roughness;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom *= PI * denom;

    return a2 / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float denom = NdotV * (1.0 - k) + k;

    return NdotV / denom;
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
    //cosTheta = min(cosTheta, 0);
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  
// ----------------------------------------------------------------------------
void main()
{
    FragColor = getData(material.albedo);
    vec3 albedo = pow(getData(material.albedo).rgb, vec3(2.2));
    
    float metallic = getData(material.metalic);

    float roughness = getData(material.roughness);

    float ao = getData(material.ao);

    vec3 N = getNormalFromMap();
    N = normalize(fs_in.normals);
    norm_ = N;
    vec3 V = normalize(fs_in.camPos - fs_in.worldPos);
    vec3 R = reflect(-V, N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);


    // reflectance equation
    vec3 Lo = vec3(0.0);
    vec3 lightPositions[1] = {
        vec3(0, 1 ,0.5)
    };
    vec3 lightColors[1] = {
        vec3(1, 1, 1)
    };
    // point lights
    for(int i = 0; i < 1; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - fs_in.worldPos);
        vec3 H = normalize(V + L);

        float distance    = length(lightPositions[i] - fs_in.worldPos);
        float attenuation = 1.0 / (distance * distance);
        attenuation = 1;
        vec3 radiance     = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular     = numerator / max(denominator, 0.001);
        
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
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    vec3 irradiance = texture(hdrMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    // Tex both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 0.0;
    vec3 prefilteredColor = textureLod(lbrMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    //prefilteredColor = vec3(1, 0, 0);
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    float shadow = ShadowCalculation(fs_in.lsPos);

    vec3 ambient    = ((kD * diffuse + specular) * ao);

    vec3 color = ambient + (1 - 0) * Lo;

    // HDR tonemapping
    color /= color + vec3(1.0);
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
    float a = getAlpha(material.albedo);
    a = 1;
    if (a < 0.1) {
        discard;
    }
    FragColor = vec4(color, a);
}