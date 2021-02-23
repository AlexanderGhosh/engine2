#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;
layout(location = 3) in vec4 ids;
layout(location = 4) in vec4 weights;

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
    vec4 ws;
} vs_out;

uniform mat4 model;
uniform mat4 bones[100];

void main() {

    // ivec4 ids = ivec4(BoneWightsIds1.xz, BoneWightsIds2.xz);
    // vec4 weights = vec4(BoneWightsIds1.yw, BoneWightsIds2.yw);
    
    mat4 boneTransform = mat4(0);

    vs_out.ws = weights; 
    if(ids[0] == -1){
        boneTransform = mat4(1);
        vs_out.ws = vec4(1, 0 , 0 , 0); 
    }
    else{
        for(int i = 0; i < 4; i++){
            int id = int(ids[i]);
            float weight = weights[i];
            boneTransform += bones[id] * weight; // bones[id]
        }
        vs_out.ws = vec4(1, 1 , 0 , 0); 
    }
    //boneTransform = mat4(1);

    gl_Position = projection * view * model * boneTransform * vec4(pos, 1);

    vs_out.worldPos = vec3(model * vec4(pos, 1));
    vs_out.normals = mat3(model) * norm;
    vs_out.camPos = viewPosition;
    vs_out.texCoords = tex;   
}