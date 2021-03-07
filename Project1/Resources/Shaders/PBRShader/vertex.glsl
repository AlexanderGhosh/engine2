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
};
layout(std140, binding = 1) uniform LSMatrices
{
    mat4 lsMatrix;
};

out VS_OUT{
    vec3 normals;
    vec2 texCoords;
    vec3 worldPos;
    vec3 camPos;
    vec4 ws;
    vec4 lsPos;
} vs_out;

uniform mat4 model;
//uniform mat4 bones[200];
uniform samplerBuffer bones;

mat4 getMatrix(int index){
    mat4 res = mat4(0);
    int x = index * 16;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            res[i][j] = texelFetch(bones, x++).x;
        }
    }
    return res;
}

void main() {

    // ivec4 ids = ivec4(BoneWightsIds1.xz, BoneWightsIds2.xz);
    // vec4 weights = vec4(BoneWightsIds1.yw, BoneWightsIds2.yw);

    mat4 boneTransform = mat4(1);

    vs_out.ws = vec4(abs(weights.x), abs(weights.y), abs(weights.z), 0);
    if(ids.x == -1/* || getMatrix(0) == mat4(0)*/){
        boneTransform = mat4(1);
        vs_out.ws = vec4(1, 1, 0, 0); 
    }
    else
    {
        for(int i = 0; i < 4; i++){
            int id = abs(int(ids[i]));
            float weight = weights[i];
            boneTransform += getMatrix(id) * weight; // bones[id]
        }
        // vs_out.ws = vec4(1, 0, 1, 0);
    }
    //boneTransform = mat4(1);

    gl_Position = projection * view * model * boneTransform * vec4(pos, 1);

    vs_out.worldPos = vec3(model * vec4(pos, 1));
    vs_out.normals = mat3(model) * norm;
    vs_out.camPos = viewPosition;
    vs_out.texCoords = tex;
    vs_out.lsPos = lsMatrix * vec4(vs_out.worldPos, 1);
}