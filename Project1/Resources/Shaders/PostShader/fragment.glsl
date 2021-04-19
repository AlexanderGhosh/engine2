#version 460 core

out vec4 FragColour;
in vec2 TextureCoords;
in float GammaValue;
in float CameraExposure;

vec3 GammaCorrect(vec3 a);
vec3 ToneMap(vec3 a);

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool Blur;

void main()
{
    vec3 hdrColor = texture(scene, TextureCoords).rgb;      
    vec3 bloomColor = texture(bloomBlur, TextureCoords).rgb;
    if(Blur)
        hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 colour = ToneMap(hdrColor);
    colour = GammaCorrect(colour);
    
    FragColour = vec4(hdrColor, 1.0);
}

vec3 GammaCorrect(vec3 col) {
    return pow(col, vec3(1.0/GammaValue));
};

vec3 ToneMap(vec3 a){
    return vec3(1.0) - exp(-a * CameraExposure); // more controll
    return a / (a + vec3(1.0)); // less controll
}