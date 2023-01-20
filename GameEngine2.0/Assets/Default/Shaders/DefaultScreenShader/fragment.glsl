
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D Tex;

void main()
{ 
    FragColor = vec4(texture(Tex, TexCoords).rgb, 1.0);
}

