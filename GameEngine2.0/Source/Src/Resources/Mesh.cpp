#include "..\..\Include\Resources\Mesh.h"
#include <map>
#include "Include/App.h"

Resources::Mesh::Mesh()
{
}

Resources::Mesh::~Mesh()
{
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
		// texture coord attribute
		glVertexAttribPointer(2U, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[6])));
		glEnableVertexAttribArray(2U);

		glVertexAttribPointer(3U, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[8])));
		glEnableVertexAttribArray(3U);
	}
	_initialized = true;
}

void Resources::Mesh::VerticesLoop(std::vector<unsigned int>& indices, std::vector<float>& vertices)
{
	std::map<int, Math::Vector3> Tangents;
	for (int k = 0; k < Indices.size(); k += 3)
	{
		Math::Vector3 Edge1 = Positions[Indices[k + (size_t)1].x] - Positions[Indices[k].x];
		Math::Vector3 Edge2 = Positions[Indices[k + (size_t)2].x] - Positions[Indices[k].x];

		float DeltaU1 = TextureUVs[Indices[k + (size_t)1].y].x - TextureUVs[Indices[k].y].x;
		float DeltaV1 = TextureUVs[Indices[k + (size_t)1].y].y - TextureUVs[Indices[k].y].y;
		float DeltaU2 = TextureUVs[Indices[k + (size_t)2].y].x - TextureUVs[Indices[k].y].x;
		float DeltaV2 = TextureUVs[Indices[k + (size_t)2].y].y - TextureUVs[Indices[k].y].y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		Math::Vector3 Tangent;

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

void Resources::Mesh::Outline(Math::Matrix4 MVP)
{
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	// Render the mesh into the stencil buffer.

	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	Update(MVP);

	// Render the thick wireframe version.

	glStencilFunc(GL_NOTEQUAL, 1, -1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glLineWidth(10);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDepthRange(0, 0.01);

	Update(MVP, true);

	glDisable(GL_STENCIL_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1);
}

void Resources::Mesh::Update(Math::Matrix4 MVP, bool outline)
{
	if (!Loaded)
		return;
	glBindVertexArray(_VAO);
	for (auto Sub : SubMeshes)
	{
		if (!Sub.Material)
			continue;
		glUniformMatrix4fv(Sub.Material->GetShader()->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
		if (outline)
		{
			glUniform1i(Sub.Material->GetShader()->GetLocation(Resources::Location::L_ENABLE_TEXTURE), false);
			glUniform4f(Sub.Material->GetShader()->GetLocation(Resources::Location::L_COLOR), 0.8f, 0.5f, 0, 1);
		}
		else
		{
			glDepthRange(0.01, 1.0);
			glUniform1i(Sub.Material->GetShader()->GetLocation(Resources::Location::L_ENABLE_TEXTURE), Sub.Material->GetTexture() ? true : false);
			if (Sub.Material->GetTexture())
				glUniform1i(Sub.Material->GetShader()->GetLocation(Location::L_TEXTURE), Sub.Material->GetTexture()->GetIndex());
			else
				glUniform4f(Sub.Material->GetShader()->GetLocation(Resources::Location::L_COLOR), Sub.Material->GetDiffuse().x, Sub.Material->GetDiffuse().y, Sub.Material->GetDiffuse().z, Sub.Material->GetDiffuse().w);
		}

		glDrawArrays(GL_TRIANGLES, (GLsizei)Sub.StartIndex, (GLsizei)Sub.Count);
	}
}