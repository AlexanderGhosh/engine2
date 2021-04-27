#version 460 core

out vec4 FragColor;

in vec2 TextureCoords;
in vec2 FragPos;
in vec2 Dimentions;
in vec2 FragPosTranslated;

struct MatItem4 {
    vec4 raw;
    sampler2D id;
    float mixValue;
    float repeatValue;
};
uniform MatItem4 albedo;
uniform float radius;
uniform vec2 parentDimentions;

vec4 getAlbedo();

const float SV = 0.7;

void main() {
    vec4 colour = getAlbedo();
    float alpha = colour.a;
    if(FragPosTranslated.x > parentDimentions.x || FragPosTranslated.y > parentDimentions.y){
        discard;
    }
    if(radius > 0.0){
        float r = radius;
        vec2 maximum = Dimentions - radius;
        if(FragPos.x > maximum.x && FragPos.y > maximum.y){
            alpha *= 1.0 - smoothstep(radius - SV, radius + SV, distance(FragPos, maximum));
        }
        else if(FragPos.x > maximum.x && FragPos.y < radius){
            alpha *= 1.0 - smoothstep(radius - SV, radius + SV, distance(FragPos, vec2(maximum.x, radius)));
        }
        else if(FragPos.x < radius && FragPos.y > maximum.y){
            alpha *= 1.0 - smoothstep(radius - SV, radius + SV, distance(FragPos, vec2(radius, maximum.y)));
        }
        else if(FragPos.x < radius && FragPos.y < radius){
            alpha *= 1.0 - smoothstep(radius - SV, radius + SV, distance(FragPos, vec2(radius)));
        }
    }
    FragColor = vec4(colour.rgb, alpha);
}
vec4 getAlbedo(){
    return mix(albedo.raw, texture(albedo.id, TextureCoords * albedo.repeatValue), albedo.mixValue);
}