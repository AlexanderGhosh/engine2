#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;
layout(location = 3) in vec4 boneIDWeights1;
layout(location = 4) in vec4 boneIDWeights2;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
    mat4 lightSpaceMatrix;
};

out VS_OUT{
    vec3 normals;
    vec2 texCoords;
    vec3 worldPos;
    vec3 camPos;
} vs_out;

uniform mat4 model;
uniform mat4 bones[100];

void main() {

    vec4 indices = vec4(boneIDWeights1.xz, boneIDWeights2.xz);
    vec4 weights = vec4(boneIDWeights1.yw, boneIDWeights2.yw);

    vec3 pos_n  = vec3(0);
    vec3 norm_n = vec3(0);

    for (int i = 0; i < 4; i++){
        mat4 bone = bones[int(indices[i])];
        bone = mat4(1);
        float weight = weights[i];
        pos_n  += vec3((bone * vec4(pos, 1)) * weight);
        norm_n += vec3((bone * vec4(norm, 1)) * weight);
    }

    gl_Position = projection * view * model * vec4(pos_n, 1);


    /*vs_out.worldPos = vec3(model * vec4(pos, 1));
    vs_out.normals = mat3(model) * norm;
    vs_out.camPos = viewPosition;
    vs_out.texCoords = tex;*/

    vs_out.worldPos = vec3(model * vec4(pos_n, 1));
    vs_out.normals = mat3(model) * norm_n;
    vs_out.camPos = viewPosition;
    vs_out.texCoords = tex;
}