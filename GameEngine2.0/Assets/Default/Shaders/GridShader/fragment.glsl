#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

#define GridSize 0.0001f
#define LineSize 0.00025f
void main()
{ 
    float x = 0.0f;
    vec2 coord = TexCoord;
    float LineWidth = LineSize / 100;
    float modx = mod(coord.x,GridSize);
    float mody = mod(coord.y,GridSize);
    bool value =  modx <= -LineWidth || modx >= + LineWidth;
    bool value2 = mody <= - LineWidth || mody >= + LineWidth;

    vec4 color = vec4(0.8, 0.8, 0.8, 0.85f );

    if (value && value2){
        discard;
    }
    else{
        float modx = mod(coord.x,GridSize * 5);
        float mody = mod(coord.y,GridSize * 5);
        if (!(modx <= -LineWidth || modx >= + LineWidth) || !(mody <= -LineWidth || mody >= + LineWidth)) 
            color = vec4(0.2, 0.2, 0.2, 0.85f);
        FragColor = color;
    }
}




