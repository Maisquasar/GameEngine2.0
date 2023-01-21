
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D Tex;

#define SIGMOID_CONTRAST 12.0

vec4 contrast(vec4 x, float s) {
    return 1.0 / (1.0 + exp(-s * (x - 0.5)));    
}

void main()
{
    FragColor = contrast(texture(Tex, TexCoords), SIGMOID_CONTRAST);
}
