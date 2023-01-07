#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 8) in mat4 instanceMatrix;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec4 OurColor;
out vec3 FragPos;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * instanceMatrix * vec4(aPos, 1.0);
	FragPos = aPos;
	TexCoord = aTexCoord;
	Normal = aNormal;
}