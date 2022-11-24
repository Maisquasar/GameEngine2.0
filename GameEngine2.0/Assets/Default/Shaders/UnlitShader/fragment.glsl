#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D Tex;
uniform bool EnableTexture;
uniform sampler2D frameBuffer;
uniform vec3 CameraPos;
uniform vec2 frameSize;
uniform vec4 OurColor;

void main()
{
    if (EnableTexture)
        FragColor = texture(Tex, TexCoord);
    else
        FragColor = vec4(OurColor.x, OurColor.y, OurColor.z, 1);
}