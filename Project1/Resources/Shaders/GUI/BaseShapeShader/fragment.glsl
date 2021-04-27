#version 460 core

out vec4 FragColor;

in vec2 TextureCoords;
in vec2 FragPos;
in vec2 Dimentions;

struct MatItem4 {
    vec4 raw;
    sampler2D id;
    float mixValue;
    float repeatValue;
};
uniform MatItem4 albedo;
uniform float radius;

vec4 getAlbedo();

void main() {
    FragColor = getAlbedo();
}
vec4 getAlbedo(){
    if(radius > 0.0){
        float r = radius;
        vec2 maximum = Dimentions - radius;
        if(FragPos.x > maximum.x && FragPos.y > maximum.y){
            if(distance(FragPos, maximum) > r){
                discard;
            }
        }
        else if(FragPos.x > maximum.x && FragPos.y < radius){
            if(distance(FragPos, vec2(maximum.x, radius)) > r){
                discard;
            }
        }
        else if(FragPos.x < radius && FragPos.y > maximum.y){
            if(distance(FragPos, vec2(radius, maximum.y)) > r){
                discard;
            }
        }
        else if(FragPos.x < radius && FragPos.y < radius){
            if(distance(FragPos, vec2(radius)) > r){
                discard;
            }
        }
    }
    return mix(albedo.raw, texture(albedo.id, TextureCoords * albedo.repeatValue), albedo.mixValue);
}