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

void Resources::BillBoard::Update(Math::Mat4 MVP, std::vector<Resources::Material*> materials, bool outline)
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

	auto up = Application.GetScene()->GetCurrentCamera()->GetTransform()->GetUpVector();
	auto right = Application.GetScene()->GetCurrentCamera()->GetTransform()->GetRightVector();
	for (int i = 0; i < materials.size(); i++)
	{
		if (!materials[i])
			continue;
		glUseProgram(materials[i]->GetShader()->Program);
		glUniformMatrix4fv(materials[i]->GetShader()->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
		glDepthRange(0.02, 1.0);
		glUniform1i(materials[i]->GetShader()->GetLocation(Location::L_ENABLE_TEXTURE), materials[i]->GetTexture() ? true : false);
		if (materials[i]->GetTexture())
			glUniform1i(materials[i]->GetShader()->GetLocation(Location::L_TEXTURE), materials[i]->GetTexture()->GetIndex());
		else
			glUniform4f(materials[i]->GetShader()->GetLocation(Location::L_COLOR), materials[i]->GetDiffuse().x, materials[i]->GetDiffuse().y, materials[i]->GetDiffuse().z, materials[i]->GetDiffuse().w);

		glUniform2f(materials[i]->GetShader()->GetLocation(Location::L_BILLSIZE), _size.x, _size.y);
		glUniform3f(materials[i]->GetShader()->GetLocation(Location::L_CAMUP), up.x, up.y, up.z);
		glUniform3f(materials[i]->GetShader()->GetLocation(Location::L_CAMRIGHT), right.x, right.y, right.z);

		glDraw(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
	}
	if (outline)
	{
		glDepthRange(0, 0.01);
		for (int i = 0; i < materials.size(); i++)
		{
			if (!materials[i])
				continue;
			glUniform1i(materials[i]->GetShader()->GetLocation(Location::L_ENABLE_TEXTURE), materials[i]->GetTexture() ? true : false);
			if (materials[i]->GetTexture())
				glUniform1i(materials[i]->GetShader()->GetLocation(Location::L_TEXTURE), materials[i]->GetTexture()->GetIndex());
			else
				glUniform4f(materials[i]->GetShader()->GetLocation(Location::L_COLOR), materials[i]->GetDiffuse().x, materials[i]->GetDiffuse().y, materials[i]->GetDiffuse().z, materials[i]->GetDiffuse().w);

			glUniform2f(materials[i]->GetShader()->GetLocation(Location::L_BILLSIZE), _size.x, _size.y);
			glUniform3f(materials[i]->GetShader()->GetLocation(Location::L_CAMUP), up.x, up.y, up.z);
			glUniform3f(materials[i]->GetShader()->GetLocation(Location::L_CAMRIGHT), right.x, right.y, right.z);
			glDraw(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
		}


		glDisable(GL_STENCIL_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1);
	}
}

void Resources::BillBoard::DrawPicking(Math::Mat4 MVP, std::vector<Resources::Material*> materials, int id)
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

	auto up = Application.GetScene()->GetCameraEditor()->GetTransform()->GetUpVector();
	auto right = Application.GetScene()->GetCameraEditor()->GetTransform()->GetRightVector();
	for (int i = 0; i < materials.size(); i++)
	{
		if (!materials[i])
			continue;
		glUniformMatrix4fv(shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);

		glUniform2f(shader->GetLocation(Location::L_BILLSIZE), _size.x, _size.y);
		glUniform3f(shader->GetLocation(Location::L_CAMUP), up.x, up.y, up.z);
		glUniform3f(shader->GetLocation(Location::L_CAMRIGHT), right.x, right.y, right.z);
		glUniform4f(shader->GetLocation(Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

		glDraw(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
	}
}
