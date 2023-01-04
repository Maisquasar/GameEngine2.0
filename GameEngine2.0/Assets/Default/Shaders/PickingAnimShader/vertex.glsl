#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 4) in vec4 boneIndices;
layout(location = 5) in vec4 boneWeights;
layout(location = 6) in vec4 boneIndices2;
layout(location = 7) in vec4 boneWeights2;

uniform mat4 ProjectionMatrix;

uniform mat4 ModelViewMatrix;

uniform int MaxBoneWeight;

const int MAX_BONES = 100;

uniform mat4 SkinningMatrices[MAX_BONES];


void main(){

    vec4 worldPosition = vec4(0.0f);
    vec4 pos = vec4(aPos, 1.0f);
    for (int i = 0; i < 4; i++)
    {
        worldPosition += (SkinningMatrices[int(boneIndices[i])] * pos) * boneWeights[i];
    }
    if (MaxBoneWeight > 4){
        for (int i = 0; i < 4; i++)
        {
            worldPosition += (SkinningMatrices[int(boneIndices2[i])] * pos) * boneWeights2[i];
        }
    }
    
    // Transform the vertex position to clip space
    gl_Position = ProjectionMatrix * (ModelViewMatrix * worldPosition);

}