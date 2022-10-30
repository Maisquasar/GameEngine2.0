#include "..\..\..\Include\Core\Components\Mesh.h"
#include <map>
#include "Include/App.h"

Core::Components::Mesh::Mesh()
{
	ComponentName = "Mesh Renderer";
}

Core::Components::Mesh::~Mesh()
{
}

void Core::Components::Mesh::ShowInInspector()
{
	if (ImGui::Button("Change Mesh"))
	{
		ImGui::OpenPopup("MeshPopup");
	}
	Resources::ResourceManager::MeshPopup();
}

void Core::Components::Mesh::Initialize()
{
	if (Loaded)
		return;

	if (Vertices.empty()) {
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		VerticesLoop(_indices, _vertices);

		this->Vertices = _vertices;
	}

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
		glVertexAttribPointer(POS, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(POS);
		// normal attribute
		glVertexAttribPointer(NML, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[3])));
		glEnableVertexAttribArray(NML);
		// texture coord attribute
		glVertexAttribPointer(TUV, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[6])));
		glEnableVertexAttribArray(TUV);

		glVertexAttribPointer(TAN, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(sizeof(float[8])));
		glEnableVertexAttribArray(TAN);
	}
	Loaded = true;
}

void Core::Components::Mesh::VerticesLoop(std::vector<unsigned int>& indices, std::vector<float>& vertices)
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

void Core::Components::Mesh::Update()
{
	if (!Loaded)
		return;
	auto MVP = App::GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	for (auto material : Materials)
	{
		glUniformMatrix4fv(material->GetShader()->GetLocation(Resources::Location::L_MVP), 1, GL_TRUE, &MVP.content[0][0]);
		glUniform1i(material->GetShader()->GetLocation(Resources::Location::L_ENABLE_TEXTURE), false);
		glUniform4f(material->GetShader()->GetLocation(Resources::Location::L_COLOR), 1, 0.5f, 0.1f, 1);

		glBindVertexArray(_VAO);
		// Temporary.
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Indices.size());
	}
}
