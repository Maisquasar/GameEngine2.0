#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 4) in vec4 boneIndices;
layout(location = 5) in vec4 boneWeights;

out vec2 TexCoord;
out vec3 Normal;
out vec4 OurColor;
out vec3 FragPos;

uniform mat4 ProjectionMatrix;

uniform mat4 ModelViewMatrix;

const int MAX_BONES = 52;

uniform mat4 SkinningMatrices[MAX_BONES];

void main()
{
	vec4 worldPosition = vec4(0.0f);
    vec4 pos = vec4(aPos, 1.0f);
    for (int i = 0; i < 4; i++)
    {
        worldPosition += (SkinningMatrices[int(boneIndices[i])] * pos) * boneWeights[i];
    }
    
    // Transform the vertex position to clip space
    gl_Position = ProjectionMatrix * (ModelViewMatrix * worldPosition);
    
    // Output the texture coordinates and normal for the fragment shader
    Normal = aNormal;
    TexCoord = aTexCoord;
}