#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D col_id;
uniform vec4 col_vec;

void main() {
    vec3 col = col_vec.rgb;
    if (col_vec.a == 0){
        col = texture(col_id, texCoords).rgb;
    }
    FragColor = vec4(1);
}