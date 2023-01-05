#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 MVP;
uniform vec2 BillSize;
uniform vec3 CamUp;
uniform vec3 CamRight;

out vec2 TexCoord;
out vec3 Normal;
out vec4 color;

void main()
{
	Normal = aNormal;
	TexCoord = vec2(aTexCoord.y,aTexCoord.x);
	vec3 wPos = CamRight * aPos.x * BillSize.x + CamUp * aPos.y * BillSize.y;
    gl_Position = MVP*vec4(wPos, 1.0f);
}