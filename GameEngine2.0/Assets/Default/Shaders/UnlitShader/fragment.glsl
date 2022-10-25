#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 OurColor;

uniform sampler2D Tex;
uniform bool EnableTexture;
uniform sampler2D frameBuffer;
uniform vec3 CameraPos;
uniform vec2 frameSize;

void main()
{
    if (EnableTexture)
        FragColor = texture(Tex, TexCoord);
    else
        FragColor = OurColor;
}