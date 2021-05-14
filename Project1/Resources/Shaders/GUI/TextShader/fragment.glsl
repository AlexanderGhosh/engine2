#version 460 core

out vec4 FragColor;

in vec2 TextureCoords;
struct MatItem4 {
    vec4 raw;
    sampler2D id;
    float mixValue;
    float repeatValue;
};
uniform sampler2D text;
uniform MatItem4 colour;
vec4 getAlbedo();

void main() {
    float alpha = texture(text, TextureCoords).r;
    vec4 col = getAlbedo();
    col.a *= alpha;
    FragColor = col;
    // FragColor = vec4(1, 0, 0, 1);
}
vec4 getAlbedo(){
    return mix(colour.raw, texture(colour.id, TextureCoords * colour.repeatValue), colour.mixValue);
}