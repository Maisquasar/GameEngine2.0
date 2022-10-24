#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 MVP;
uniform vec4 Color;

out vec2 TexCoord;
out vec3 Normal;
out vec4 OurColor;
out vec3 FragPos;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	FragPos = aPos;
	TexCoord = aTexCoord;
	Normal = aNormal;
	OurColor = Color;
}