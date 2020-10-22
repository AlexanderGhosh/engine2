#version 460 core

out vec4 FragCol;
in vec3 fragPos;
in vec2 texCoords;

struct Material {
     vec4 diffuse_vec;
     sampler2D diffuse_id;

     vec4 specular_vec;
     sampler2D specular_id;

     float shininess;
};

uniform Material material;

uniform sampler2D t;

void main() {
     vec3 col = material.diffuse_vec.rgb;
     if (material.diffuse_vec.a == 0){
          col = texture(material.diffuse_id, texCoords).rgb;
     }
     // col = vec3(1, 0, 0);
     FragCol = vec4(col, 1);
}