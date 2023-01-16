#include <Windows.h>
#include <glad/glad.h>

#include "Include/Resources/SkeletalMesh.h"
#include "Include/Resources/Material.h"
#include "Include/Resources/Shader.h"
#include "Include/Resources/Skeleton.h"
#include "Include/App.h"

Resources::SkeletalMesh::SkeletalMesh()
{
}

Resources::SkeletalMesh::~SkeletalMesh()
{
}

void Resources::SkeletalMesh::Initialize()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	//glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	if (!Vertices.empty()) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

		bool over4 = MAX_BONE_WEIGHT > 4;
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

		int size = 3 + 3 + 2 + 3 + (over4 ? 16 : 8);

		// position attribute
		glVertexAttribPointer(0U, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0U);
		// normal attribute
		glVertexAttribPointer(1U, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(sizeof(float[3])));
		glEnableVertexAttribArray(1U);
		// texture coord attribute
		glVertexAttribPointer(2U, 2, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(sizeof(float[6])));
		glEnableVertexAttribArray(2U);

		glVertexAttribPointer(3U, 3, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(sizeof(float[8])));
		glEnableVertexAttribArray(3U);

		glVertexAttribPointer(4U, 4, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(sizeof(float[11])));
		glEnableVertexAttribArray(4U);

		glVertexAttribPointer(5U, 4, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(sizeof(float[15])));
		glEnableVertexAttribArray(5U);
		if (over4) {

			glVertexAttribPointer(6U, 4, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(sizeof(float[19])));
			glEnableVertexAttribArray(6U);

			glVertexAttribPointer(7U, 4, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)(sizeof(float[23])));
			glEnableVertexAttribArray(7U);
		}
	}
	_initialized = true;
}

void Resources::SkeletalMesh::Update(Math::Mat4 M, Skeleton* skel, bool outline)
{
	if (!Loaded || !skel)
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

	auto MV = Application.GetScene()->GetCameraEditor()->GetViewMatrix();
	auto P = Application.GetScene()->GetCameraEditor()->GetProjection();
	for (auto Sub : SubMeshes)
	{
		if (!Sub.Material)
			continue;
		glDepthRange(0.01, 1.0);
		glUseProgram(Sub.Material->GetShader()->Program);
		glUniform1i(Sub.Material->GetShader()->GetLocation(Resources::Location::L_MAXBONEWEIGHT), skel->GetMaxBoneWeight());
		glUniformMatrix4fv(Sub.Material->GetShader()->GetLocation(Resources::Location::L_MODELVIEWMATRIX), 1, GL_TRUE, &MV.content[0][0]);
		glUniformMatrix4fv(Sub.Material->GetShader()->GetLocation(Resources::Location::L_PROJECTIONMATRIX), 1, GL_TRUE, &P.content[0][0]);
		glUniformMatrix4fv(Sub.Material->GetShader()->GetLocation(Resources::Location::L_SKINNINGMATRICES), (GLsizei)skel->Bones.size(), GL_TRUE, &skel->GetBonesMatrices().data()->content[0][0]);

		glUniform1i(Sub.Material->GetShader()->GetLocation(Resources::Location::L_ENABLE_TEXTURE), Sub.Material->GetTexture() ? true : false);
		if (Sub.Material->GetTexture())
			glUniform1i(Sub.Material->GetShader()->GetLocation(Location::L_TEXTURE), Sub.Material->GetTexture()->GetIndex());
		else
			glUniform4f(Sub.Material->GetShader()->GetLocation(Resources::Location::L_COLOR), Sub.Material->GetDiffuse().x, Sub.Material->GetDiffuse().y, Sub.Material->GetDiffuse().z, Sub.Material->GetDiffuse().w);

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
			glUniform1i(Sub.Material->GetShader()->GetLocation(Resources::Location::L_ENABLE_TEXTURE), false);
			glUniform4f(Sub.Material->GetShader()->GetLocation(Resources::Location::L_COLOR), 0.8f, 0.5f, 0, 1);
			glDraw(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
		}


		glDisable(GL_STENCIL_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1);
	}
}

void Resources::SkeletalMesh::DrawPicking(Math::Mat4 M, Skeleton* skel, int id)
{
	if (!Loaded)
		return;
	if (!_initialized)
		Initialize();
	auto picking_shader = Application.GetResourceManager()->GetPickingAnimShader();
	glBindVertexArray(_VAO);
	glUseProgram(picking_shader->Program);
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;
	auto MV = Application.GetScene()->GetCameraEditor()->GetViewMatrix();
	auto P = Application.GetScene()->GetCameraEditor()->GetProjection();
	for (const auto& Sub : this->SubMeshes)
	{
		glUniformMatrix4fv(picking_shader->GetLocation(Resources::Location::L_MODELVIEWMATRIX), 1, GL_TRUE, &MV.content[0][0]);
		glUniformMatrix4fv(picking_shader->GetLocation(Resources::Location::L_PROJECTIONMATRIX), 1, GL_TRUE, &P.content[0][0]);
		glUniformMatrix4fv(picking_shader->GetLocation(Resources::Location::L_SKINNINGMATRICES), (GLsizei)skel->Bones.size(), GL_TRUE, &skel->GetBonesMatrices().data()->content[0][0]);
		glUniform4f(picking_shader->GetLocation(Resources::Location::L_COLOR), r / 255.f, g / 255.f, b / 255.f, 1.f);

		glDraw(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
	}
}