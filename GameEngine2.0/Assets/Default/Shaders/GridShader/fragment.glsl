#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

#define GridSize 0.0001f
#define LineSize 0.0001
void main()
{ 
    float x = 0.0f;
    vec2 coord = TexCoord;
    float LineWidth = LineSize / 100;
    bool value = mod(coord.x,GridSize) <= x - LineWidth || mod(coord.x,GridSize) >= x + LineWidth;
    bool value2 = mod(coord.y,GridSize) <= x - LineWidth || mod(coord.y,GridSize) >= x + LineWidth;



    if (value && value2)
        FragColor = vec4(0, 0, 0, 0);
    else
        FragColor = vec4(1, 1, 1, 0.65 );
}