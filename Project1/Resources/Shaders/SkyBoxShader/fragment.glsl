#version 460 core

out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main() {
    vec3 colour = texture(skybox, texCoords).rgb;
    
    // colour = colour / (colour + vec3(1.0));
    // colour = pow(colour, vec3(1.0/2.2)); 
  
    FragColor = vec4(colour, 1.0);
}