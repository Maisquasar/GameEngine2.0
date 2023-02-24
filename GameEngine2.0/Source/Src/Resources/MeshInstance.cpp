#include "stdafx.h"
#include "..\..\Include\Resources\MeshInstance.h"
#include "Include/App.h"

Resources::MeshInstance::MeshInstance()
{
}

Resources::MeshInstance::~MeshInstance()
{
}

void Resources::MeshInstance::Initialize(int buf)
{
	Mesh::Initialize();
	if (buf != -1)
		glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBindVertexArray(_VAO);
	// vertex attributes

	GLsizei vec4Size = sizeof(float[4]);
	glEnableVertexAttribArray(8U);
	glVertexAttribPointer(8U, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);

	glVertexAttribDivisor(8U, 1);

	glBindVertexArray(0);
}

void Resources::MeshInstance::Draw(int amount)
{
	glDepthRange(0.02, 1.0);
	glBindVertexArray(_VAO);
	Application.GetSettings()->NumberOfInstancedTriangles += (this->SubMeshes[0].Count / 3) * amount;
	Application.GetSettings()->NumberOfInstances += amount;
	glDrawArraysInstanced(GL_TRIANGLES, 0, (GLsizei)this->SubMeshes[0].Count, amount);
	glBindVertexArray(0);
}

