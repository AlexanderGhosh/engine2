#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normals;
    vec2 texCoords;
    vec3 viewPos;
    float gammaValue;
    vec4 fragPosLightSpace;
} fs_in;

struct Material {
     vec4 diffuse_vec;
     sampler2D diffuse_id;

     vec4 specular_vec;
     sampler2D specular_id;

     vec4 normals_vec;
     sampler2D normals_id;
     
     float shininess;
};

uniform sampler2D depthMap;
uniform Material material;

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

void main() {
     // FragColor = vec4(vec3(gl_FragCoord.z), 1);
     
     float depthValue = texture(depthMap, fs_in.texCoords).r;
     
     vec3 lightPos = vec3(0, 0, 0);

     vec3 diff_ = material.diffuse_vec.rgb;
     if (material.diffuse_vec.a == 0){
          diff_ = texture(material.diffuse_id, fs_in.texCoords).rgb;
     }

     vec3 spec_ = material.specular_vec.rgb;
     if (material.specular_vec.a == 0){
          spec_ = texture(material.specular_id, fs_in.texCoords).rgb;
     }

     vec3 norm_ = fs_in.normals;
     if (material.normals_vec.a == 0){
          norm_ = texture(material.normals_id, fs_in.texCoords).rgb;
     }


     vec3 color = diff_;
     // ambient
     vec3 ambient = 0.05 * color;
     // diffuse
     vec3 lightDir = normalize(lightPos - fs_in.fragPos);
     vec3 normal = normalize(norm_);
     float diff = max(dot(lightDir, normal), 0.0);
     vec3 diffuse = diff * color;
     // specular
     vec3 viewDir = normalize(fs_in.viewPos - fs_in.fragPos);
     vec3 reflectDir = reflect(-lightDir, normal);
     float spec = 0.0;
     
     vec3 halfwayDir = normalize(lightDir + viewDir);  
     spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

     vec3 specular = spec_ * spec; // assuming bright white light color


     float shadow = ShadowCalculation(fs_in.fragPosLightSpace);       
     vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
     lighting = pow(lighting, vec3(1.0 / fs_in.gammaValue));

     FragColor = vec4(sum, 1.0);
     // FragColor = vec4(diff_, 1);
     
}