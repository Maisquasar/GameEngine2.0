#include "..\..\Include\Resources\BillBoard.h"

#include <Windows.h>
#include <glad/glad.h>
#include "Include/Resources/Material.h"
#include "Include/Resources/Shader.h"
#include "Include/App.h"

Resources::BillBoard::BillBoard()
{
}

Resources::BillBoard::~BillBoard()
{
}

void Resources::BillBoard::Update(Math::Matrix4 MVP, bool outline)
{
	if (!Loaded)
		return;
	if (!_initialized)
		Initialize();
	glBindVertexArray(_VAO);
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	// Render the mesh into the stencil buffer.

	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	auto up = Application.GetScene()->GetCameraEditor()->Transform.GetUpVector();
	auto right = Application.GetScene()->GetCameraEditor()->Transform.GetRightVector();
	for (auto Sub : SubMeshes)
	{
		if (!Sub.Material)
			continue;
		glUseProgram(Sub.Material->GetShader()->Program);
		glUniformMatrix4fv(Sub.Material->GetShader()->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
		glDepthRange(0.01, 1.0);
		glUniform1i(Sub.Material->GetShader()->GetLocation(Location::L_ENABLE_TEXTURE), Sub.Material->GetTexture() ? true : false);
		if (Sub.Material->GetTexture())
			glUniform1i(Sub.Material->GetShader()->GetLocation(Location::L_TEXTURE), Sub.Material->GetTexture()->GetIndex());
		else
			glUniform4f(Sub.Material->GetShader()->GetLocation(Location::L_COLOR), Sub.Material->GetDiffuse().x, Sub.Material->GetDiffuse().y, Sub.Material->GetDiffuse().z, Sub.Material->GetDiffuse().w);
		
		glUniform2f(Sub.Material->GetShader()->GetLocation(Location::L_BILLSIZE), _size.x, _size.y);
		glUniform3f(Sub.Material->GetShader()->GetLocation(Location::L_CAMUP), up.x, up.y, up.z);
		glUniform3f(Sub.Material->GetShader()->GetLocation(Location::L_CAMRIGHT), right.x, right.y, right.z);

		glDraw(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
	}
	if (outline)
	{
		// Render the thick wireframe version.
		glStencilFunc(GL_NOTEQUAL, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glLineWidth(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDepthRange(0, 0.01);

		for (auto Sub : SubMeshes)
		{
			if (!Sub.Material)
				continue;
			glUniform1i(Sub.Material->GetShader()->GetLocation(Location::L_ENABLE_TEXTURE), false);
			glUniform4f(Sub.Material->GetShader()->GetLocation(Location::L_COLOR), 0.8f, 0.5f, 0, 1);		
			glUniform2f(Sub.Material->GetShader()->GetLocation(Location::L_BILLSIZE), _size.x, _size.y);
			glUniform3f(Sub.Material->GetShader()->GetLocation(Location::L_CAMUP), up.x, up.y, up.z);
			glUniform3f(Sub.Material->GetShader()->GetLocation(Location::L_CAMRIGHT), right.x, right.y, right.z);
			glDraw(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
		}


		glDisable(GL_STENCIL_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1);
	}
}

void Resources::BillBoard::DrawPicking(Math::Matrix4 MVP, int id)
{
	if (!Loaded)
		return;
	if (!_initialized)
		Initialize();
	glBindVertexArray(_VAO);
	auto shader = Application.GetResourceManager()->GetBillboardPickingShader();
	if (!shader)
		return;
	glUseProgram(shader->Program);
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;

	auto up = Application.GetScene()->GetCameraEditor()->Transform.GetUpVector();
	auto right = Application.GetScene()->GetCameraEditor()->Transform.GetRightVector();
	for (const auto& Sub : this->SubMeshes)
	{
		if (!Sub.Material)
			continue;
		glUniformMatrix4fv(shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);

		glUniform2f(shader->GetLocation(Location::L_BILLSIZE), _size.x, _size.y);
		glUniform3f(shader->GetLocation(Location::L_CAMUP), up.x, up.y, up.z);
		glUniform3f(shader->GetLocation(Location::L_CAMRIGHT), right.x, right.y, right.z);
		glUniform4f(shader->GetLocation(Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

		glDraw(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
	}
}
