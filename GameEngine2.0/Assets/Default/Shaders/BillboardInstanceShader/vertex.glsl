#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 8) in vec4 xyzs;

uniform vec3 CamUp;
uniform vec3 CamRight;
uniform mat4 ViewProjectionMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec4 OurColor;

void main()
{

	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter = xyzs.xyz;
	vec3 wPos = particleCenter + CamRight * aPos.x * xyzs.w + CamUp * aPos.y * xyzs.w;
	gl_Position = ViewProjectionMatrix * vec4(wPos, 1.0);
	TexCoord = aTexCoord;
	Normal = aNormal;
}
