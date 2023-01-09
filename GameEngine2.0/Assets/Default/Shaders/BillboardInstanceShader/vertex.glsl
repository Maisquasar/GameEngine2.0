#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 8) in mat4 instanceMatrix;

uniform vec2 BillSize;
uniform vec3 CamUp;
uniform vec3 CamRight;
// uniform mat4 ProjectionMatrix;
// uniform mat4 ViewMatrix;
uniform mat4 ViewProjectionMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec4 OurColor;

void main()
{
	vec3 wPos = CamRight * aPos.x * BillSize.x + CamUp * aPos.y * BillSize.y;
	gl_Position = ViewProjectionMatrix * instanceMatrix * vec4(wPos, 1.0);
	TexCoord = aTexCoord;
	Normal = aNormal;
}