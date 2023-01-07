#include "..\..\Include\Resources\MeshInstance.h"

Resources::MeshInstance::MeshInstance()
{
}

Resources::MeshInstance::~MeshInstance()
{
}

void Resources::MeshInstance::Initialize()
{
	if (!IsInitialized())
		Mesh::Initialize();
    glBindVertexArray(_VAO);
    // vertex attributes

    GLsizei vec4Size = sizeof(float[4]);
    glEnableVertexAttribArray(8U);
    glVertexAttribPointer(8U, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(9U);
    glVertexAttribPointer(9U, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(sizeof(float[4])));
    glEnableVertexAttribArray(10U);
    glVertexAttribPointer(10U, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(sizeof(float[8])));
    glEnableVertexAttribArray(11U);
    glVertexAttribPointer(11U, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(sizeof(float[12])));

    glVertexAttribDivisor(8U, 1);
    glVertexAttribDivisor(9U, 1);
    glVertexAttribDivisor(10U, 1);
    glVertexAttribDivisor(11U, 1);

    glBindVertexArray(0);
}

void Resources::MeshInstance::Draw(Shader* shader, int amount)
{
    glBindVertexArray(_VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)this->SubMeshes[0].Count, amount);
    glBindVertexArray(0);
}

