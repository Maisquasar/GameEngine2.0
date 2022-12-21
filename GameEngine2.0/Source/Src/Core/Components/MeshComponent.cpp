#include "Include/Core/Components/MeshComponent.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/Resources/Mesh.h"
#include "Include/Core/Node.h"
#include "Include/Utils/Utils.h"
#include "Include/Utils/Loader.h"

Core::Components::MeshComponent::MeshComponent()
{
	ComponentName = "MeshComponent";
}

Core::Components::MeshComponent::~MeshComponent()
{
	delete _currentMesh;
}

float getDistance(Math::Vector3 point1, Math::Vector3 point2, Math::Vector3 axis)
{
	// Project the points onto the custom axis
	float dot1 = point1.DotProduct(axis);
	float dot2 = point2.DotProduct(axis);

	// Calculate the distance between the projected points
	return abs(dot1 - dot2);
}

void Core::Components::MeshComponent::ShowInInspector()
{
	Super::ShowInInspector();
	if (ImGui::Button("Change Mesh"))
	{
		ImGui::OpenPopup("MeshPopup");
	}
	ImGui::SameLine();
	if (GetMesh())
		ImGui::Text(GetMesh()->GetName().c_str());
	else
		ImGui::Text("None");
	if (auto Mesh = Application.GetResourceManager()->ResourcesPopup<Resources::Mesh>("MeshPopup")) {
		delete _currentMesh;
		_currentMesh = Mesh->Clone();
	}
	if (!GetMesh())
		return;

	// Material List.
	static Resources::SubMesh* ChangeMat = nullptr;
	static int SelectedRow = 0;
	if (ImGui::BeginTable("Materials", 3, ImGuiTableFlags_Borders))
	{
		for (int row = 0; row < GetMesh()->SubMeshes.size(); row++)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < 3; column++)
			{
				ImGui::TableSetColumnIndex(column);
				switch (column)
				{
				case 0:
					ImGui::Text("%d", row);
					break;
				case 1:
					if (GetMesh()->SubMeshes[row].Material)
						ImGui::Text(GetMesh()->SubMeshes[row].Material->GetName().c_str());
					else
						ImGui::Text("Missing Material");
					break;
				case 2:
					ImGui::PushID(row);
					if (ImGui::Button("Change Material"))
					{
						ChangeMat = &GetMesh()->SubMeshes[row];
						SelectedRow = row;
						ImGui::OpenPopup("MaterialPopup");
					}
					ImGui::PopID();
					break;
				}
			}
		}
		ImGui::PushID(SelectedRow);
		if (auto mat = Application.GetResourceManager()->ResourcesPopup<Resources::Material>("MaterialPopup")) {
			ChangeMat->Material = mat;
		}
		ImGui::PopID();
		ImGui::EndTable();
	}

	// Material Comnponents List.
	int index = 0;
	for (auto Sub : GetMesh()->SubMeshes)
	{
		ImGui::PushID(index++);
		ImGui::Separator();
		if (ImGui::CollapsingHeader(Sub.Material->GetName().c_str())) {

			ImGui::BeginDisabled(!Sub.Material->IsEditable());

			// Shader
			if (ImGui::Button("Shader"))
			{
				ImGui::OpenPopup("ShaderPopup");
			}
			if (auto sha = Application.GetResourceManager()->ResourcesPopup<Resources::Shader>("ShaderPopup"))
				Sub.Material->SetShader(sha);
			ImGui::SameLine();
			ImGui::Text("%s", Sub.Material->GetShader()->GetName().c_str());

			// Texture
			if (ImGui::Button("Change Texture"))
			{
				ImGui::OpenPopup("TexturePopup");
			}
			if (auto tex = Application.GetResourceManager()->ResourcesPopup<Resources::Texture>("TexturePopup"))
				Sub.Material->SetTexture(tex);
			ImGui::SameLine();
			if (Sub.Material->GetTexture())
			{
				ImGui::Text(Sub.Material->GetTexture()->GetName().c_str());
			}
			else
				ImGui::Text("None");
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				Sub.Material->SetTexture(nullptr);
			}

			// Parameters
			ImGui::ColorEdit4("Ambient", &(*Sub.Material->GetPtrAmbient())[0]);
			ImGui::ColorEdit4("Diffuse", &(*Sub.Material->GetPtrDiffuse())[0]);
			ImGui::ColorEdit4("Specular", &(*Sub.Material->GetPtrSpecular())[0]);
			ImGui::EndDisabled();
		}
		ImGui::PopID();
	}
}

void Core::Components::MeshComponent::Update()
{
	if (!_currentMesh || !_enable)
		return;
	auto MVP = Application.GetScene()->GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	if (_currentMesh)
		GetMesh()->Update(MVP, EditorUi::Editor::GetInspector()->IsSelected(GameObject));
}

void Core::Components::MeshComponent::DrawPicking(int id)
{
	if (!_currentMesh || !_enable)
		return;
	auto MVP = Application.GetScene()->GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	if (_currentMesh)
		GetMesh()->DrawPicking(MVP, id);
}

Resources::Mesh* Core::Components::MeshComponent::GetMesh()
{
	return dynamic_cast<Resources::Mesh*>(_currentMesh);
}

void Core::Components::MeshComponent::SetMesh(Resources::IResource* mesh)
{
	_currentMesh = mesh;
}

void Core::Components::MeshComponent::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Mesh : %s\n", GetMesh()->GetPath().c_str());
	int index = 0;
	for (auto Sub : GetMesh()->SubMeshes)
	{
		content += space + Utils::StringFormat("SubMesh[%d] : %s\n", index++, Sub.Material->GetPath().c_str());
	}
}
void Core::Components::MeshComponent::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	int SubMeshIndex = 0;
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 4) == "Mesh")
		{
			auto MeshPath = Utils::Loader::GetString(currentLine);
			if (auto mesh = Application.GetResourceManager()->Get<Resources::Mesh>(MeshPath.c_str())) {
				this->_currentMesh = mesh->Clone();
			}
			else
			{
				_currentMesh = new Resources::Mesh();
				_currentMesh->SetPath(MeshPath);
#if MULTITHREAD_LOADING
				Application.MultiThreadMeshes.push_back(&_currentMesh);
#endif
			}
		}
		else if (currentLine.substr(0, 7) == "SubMesh")
		{
			auto SubMeshMaterialPath = Utils::Loader::GetString(currentLine);
			if (auto mat = Application.GetResourceManager()->Get<Resources::Material>(SubMeshMaterialPath.c_str())) {
				if (GetMesh()->SubMeshes.size() <= SubMeshIndex) {
					GetMesh()->SubMeshes.push_back(Resources::SubMesh());
				}
				GetMesh()->SubMeshes[SubMeshIndex].Material = mat;
				SubMeshIndex++;
			}
		}
		pos++;
	}
}
