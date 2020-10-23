#version 460 core

out vec4 FragColor;

in VS_OUT {
    vec3 fragPos;
    vec3 normals;
    vec2 texCoords;
    vec3 viewPos;
    float gammaValue;
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

uniform Material material;

void main() {
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
     vec3 sum = ambient + diffuse + specular;
     sum *= 1;

     sum = pow(sum, vec3(1.0 / fs_in.gammaValue));

     FragColor = vec4(sum, 1.0);
     // FragColor = vec4(diff_, 1);
}