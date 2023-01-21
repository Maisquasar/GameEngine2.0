#include "..\..\Include\Resources\Mesh.h"
#include <map>
#include "Include/App.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/Resources/Material.h"

Resources::Mesh::Mesh()
{
}

Resources::Mesh::~Mesh()
{
	if (_VAO != 0)
		glDeleteVertexArrays(1, &_VAO);
	if (_VBO != 0)
		glDeleteBuffers(1, &_VBO);
}

void Resources::Mesh::Load(std::string filename)
{
	if (Loaded)
		return;

	if (Vertices.empty()) {
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		VerticesLoop(_indices, _vertices);

		this->Vertices = _vertices;
	}
	Loaded = true;

#if !MULTITHREAD_LOADING
	Initialize();
#endif
}

void Resources::Mesh::Initialize()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	//glGenBuffers(1, &EBO);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	if (!Vertices.empty()) {
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0U, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0U);
		// normal attribute
		glVertexAttribPointer(1U, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[3])));
		glEnableVertexAttribArray(1U);
		// texture coordinates attribute
		glVertexAttribPointer(2U, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[6])));
		glEnableVertexAttribArray(2U);

		glVertexAttribPointer(3U, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[8])));
		glEnableVertexAttribArray(3U);
	}
	_initialized = true;
}

void Resources::Mesh::VerticesLoop(std::vector<unsigned int>& indices, std::vector<float>& vertices)
{
	std::map<int, Math::Vec3> Tangents;
	for (int k = 0; k < Indices.size(); k += 3)
	{
		Math::Vec3 Edge1 = Positions[Indices[k + (size_t)1].x] - Positions[Indices[k].x];
		Math::Vec3 Edge2 = Positions[Indices[k + (size_t)2].x] - Positions[Indices[k].x];

		float DeltaU1 = TextureUVs[Indices[k + (size_t)1].y].x - TextureUVs[Indices[k].y].x;
		float DeltaV1 = TextureUVs[Indices[k + (size_t)1].y].y - TextureUVs[Indices[k].y].y;
		float DeltaU2 = TextureUVs[Indices[k + (size_t)2].y].x - TextureUVs[Indices[k].y].x;
		float DeltaV2 = TextureUVs[Indices[k + (size_t)2].y].y - TextureUVs[Indices[k].y].y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		Math::Vec3 Tangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		Tangents[k] = Tangents[k] + Tangent;
		Tangents[k + 1] = Tangents[k] + Tangent;
		Tangents[k + 2] = Tangents[k] + Tangent;
	}
	for (int k = 0; k < Indices.size(); k++)
	{
		vertices.push_back(Positions[Indices[k].x].x);
		vertices.push_back(Positions[Indices[k].x].y);
		vertices.push_back(Positions[Indices[k].x].z);

		vertices.push_back(Normals[Indices[k].z].x);
		vertices.push_back(Normals[Indices[k].z].y);
		vertices.push_back(Normals[Indices[k].z].z);

		vertices.push_back(TextureUVs[Indices[k].y].x);
		vertices.push_back(TextureUVs[Indices[k].y].y);

		vertices.push_back(Tangents[k].x);
		vertices.push_back(Tangents[k].y);
		vertices.push_back(Tangents[k].z);
	}
	
}

void glDraw(uint32_t model, GLsizei start, GLsizei count)
{
	glDrawArrays(model, (GLsizei)start, (GLsizei)count);
	switch (model)
	{
	case GL_TRIANGLES:
		Application.GetSettings()->NumberOfTrianglesDraw += count / 3;
		break;
	default:
		break;
	}
	Application.GetSettings()->NumberOfDrawCalls++;
}

void Resources::Mesh::Update(Math::Mat4 MVP, std::vector<Resources::Material*> materials, bool outline)
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

		if (ShouldDrawCall)
			glDraw(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
		else
			glDrawArrays(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
	}
	if (outline && Application.GetScene()->GetCurrentCamera() == Application.GetScene()->GetCameraEditor())
	{

		// Render the thick wire frame version.
		glStencilFunc(GL_NOTEQUAL, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glLineWidth(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDepthRange(0, 0.01);

		for (int i = 0; i < materials.size(); i++)
		{
			if (!materials[i])
				continue;
			glUniform1i(materials[i]->GetShader()->GetLocation(Location::L_ENABLE_TEXTURE), false);
			glUniform4f(materials[i]->GetShader()->GetLocation(Location::L_COLOR), 0.8f, 0.5f, 0, 1);
			if (ShouldDrawCall)
				glDraw(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
			else
				glDrawArrays(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
		}


		glDisable(GL_STENCIL_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1);
	}
}

void Resources::Mesh::DrawPicking(Math::Mat4 MVP, std::vector<Resources::Material*> materials, int id)
{
	if (!Loaded)
		return;
	if (!_initialized)
		Initialize();
	glBindVertexArray(_VAO);
	static auto shader = Application.GetResourceManager()->GetPickingShader();
	if (!shader)
		return;
	glUseProgram(shader->Program);
	int r = (id & 0x000000FF) >> 0;
	int g = (id & 0x0000FF00) >> 8;
	int b = (id & 0x00FF0000) >> 16;
	for (int i = 0; i < materials.size(); i++)
	{
		if (!materials[i])
			continue;
		glUniformMatrix4fv(shader->GetLocation(Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
		glUniform4f(shader->GetLocation(Location::L_COLOR), r/255.f, g/255.f, b/255.f, 1.f);

		if (ShouldDrawCall)
			glDraw(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
		else
			glDrawArrays(GL_TRIANGLES, (GLsizei)SubMeshes[i].StartIndex, (GLsizei)SubMeshes[i].Count);
	}
}
