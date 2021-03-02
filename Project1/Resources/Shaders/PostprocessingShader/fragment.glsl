#version 460 core

out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D shadows;

in vec2 tex_coords;

void main(){
    //texture(scene, tex_coords).xyz
    FragColor = vec4(texture(scene, tex_coords).rgb * 0.95, 1);
}