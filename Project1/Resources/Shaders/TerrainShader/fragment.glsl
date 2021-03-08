#version 460 core

out vec4 FragColor;

in vec3 norm;
in vec2 texCoords;
in float height;

uniform sampler2D textures[3];

float map(float val, float low, float high, float l, float h){
    return l + ((h - l) / (high - low)) * (val - low);
}

void main() {

    // if you want multiple textures...0
    vec3 t1 = vec3(0, 1, 0); // texture(textures[0], texCoords);
    vec3 t2 = vec3(139,69,19) / 255; // texture(textures[1], texCoords);
    vec3 t3 = vec3(0.5); // texture(textures[2], texCoords);
    
    // Blend cliff texture using the y component of the normal
    float r = map(height, -0.5, 0.5, 0, 1);
    //r = 1.0 / (1.0 - r) - 1;
    vec3 col = mix(t2, t1, r);


    FragColor = vec4(vec3(r), 1);
}