#version 460 core

out vec4 FragColor;

in vec3 norm;
in vec2 texCoords;
in float height;

struct MatItem3{
    vec3 raw;
    sampler2D id;
    float mixValue;
    float repeatValue;
};
vec3 getData(MatItem3 item){
    return mix(item.raw, texture(item.id, texCoords * item.repeatValue).rgb, item.mixValue);
};

uniform MatItem3 lowest;
uniform MatItem3 middle;
uniform MatItem3 highest;


vec3 mix3(vec3 a, vec3 b, vec3 c){
    return 
    a * ((1.0 - 1.0 * height) * 0.25) + 
    b * ((3.0 - 3.0 * height) * 0.25) + 
    c * height;
}

void main() {

    vec3 low = getData(lowest);
    vec3 mid = getData(middle);
    vec3 high = getData(highest);
    // mix3(low, mid, high)
    FragColor = vec4(mix3(low, mid, high), 1);
}