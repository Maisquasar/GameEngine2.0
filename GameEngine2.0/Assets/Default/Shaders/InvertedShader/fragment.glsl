
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D Tex;

void main()
{ 
    FragColor = vec4(vec3(1.0 - texture(Tex, TexCoords)), 1.0);
}

