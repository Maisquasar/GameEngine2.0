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
    FragColor = vec4(1, 1, 1, 1);
}