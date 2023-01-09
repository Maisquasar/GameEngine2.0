#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D Tex;
uniform bool EnableTexture;
uniform vec4 OurColor;

void main()
{ 
    if (EnableTexture)
    {
        vec4 color = texture(Tex, TexCoord);
        if (color.a < 0.5) discard;
        FragColor = color;
    }
    else
        FragColor = vec4(OurColor.x, OurColor.y, OurColor.z, 1);
}