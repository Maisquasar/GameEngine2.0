#include "..\..\Include\Resources\MeshInstance.h"
#include "Include/App.h"

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

	glVertexAttribDivisor(8U, 1);

	glBindVertexArray(0);
}

void Resources::MeshInstance::Draw(Shader* shader, int amount)
{
	glBindVertexArray(_VAO);
	Application.GetSettings()->NumberOfInstancedTriangles += (this->SubMeshes[0].Count/3) * amount;
	Application.GetSettings()->NumberOfInstances += amount;
	glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)this->SubMeshes[0].Count, amount);
	glBindVertexArray(0);
}

