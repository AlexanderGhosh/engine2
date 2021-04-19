#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

layout(std140, binding = 0) uniform Matrices
{
    mat4 view;
    mat4 projection;
    vec3 viewPosition;
    float gamma;
};
uniform mat4 model;
uniform sampler2D hm;

float map(float val, float low, float high, float l, float h){
    return l + ((h - l) / (high - low)) * (val - low);
}
float calcHeight(vec2 pos){
    return map(texture(hm, pos).r, 0, 1, -0.5, 0.5);
}
vec3 calcNormal(vec2 position){
    // # P.xy store the position for which we want to calculate the normals
    // # height() here is a function that return the height at a point in the terrain

    // read neightbor heights using an arbitrary small offset
    vec3 off = vec3(1.0, 1.0, 0.0);
    float hL = calcHeight(position - off.xz);
    float hR = calcHeight(position + off.xz);
    float hD = calcHeight(position - off.zy);
    float hU = calcHeight(position + off.zy);

    // deduce terrain normal
    vec3 normal = vec3(0);
    normal.x = hL - hR;
    normal.y = 2.0;
    normal.z = hD - hU;
    return normalize(normal);
}

out vec2 TexCoords;
out vec3 WorldPosition;
out vec3 Normal;

void main() {
    vec3 position = pos;
    position.y = calcHeight(tex);
    // height = position.y;
    WorldPosition = vec3(model * vec4(position, 1));
	gl_Position = projection * view * model * vec4(position, 1);
    TexCoords = tex;
    Normal = calcNormal(position.xz);
}