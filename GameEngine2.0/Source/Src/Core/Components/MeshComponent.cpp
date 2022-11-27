#include "Include/Core/Components/MeshComponent.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"

Core::Components::MeshComponent::MeshComponent()
{
	ComponentName = "MeshComponent";
}

Core::Components::MeshComponent::~MeshComponent() 
{
	delete _currentMesh;
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
	if (auto Mesh = Resources::ResourceManager::ResourcesPopup<Resources::Mesh>("MeshPopup"))
		_currentMesh = Mesh->Clone();
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
					ImGui::Text(GetMesh()->SubMeshes[row].Material->GetName().c_str());
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
		if (auto mat = Resources::ResourceManager::ResourcesPopup<Resources::Material>("MaterialPopup")) {
			ChangeMat->Material = mat;
		}
		ImGui::PopID();
		ImGui::EndTable();
	}

	// Material Comnponents List.
	int index = 0;
	for (auto Sub : GetMesh()->SubMeshes)
	{
		if (!Sub.Material)
			continue;
		ImGui::PushID(index++);
		ImGui::Separator();
		if (ImGui::CollapsingHeader(Sub.Material->GetName().c_str())) {

			ImGui::BeginDisabled(!Sub.Material->IsEditable());
			ImGui::Button("Shader");
			ImGui::SameLine();
			ImGui::Text("%s", Sub.Material->GetShader()->GetName().c_str());

			ImGui::Button("Texture");
			ImGui::SameLine();
			if (Sub.Material->GetTexture())
			{
				ImGui::Text(Sub.Material->GetTexture()->GetName().c_str());
			}
			else
				ImGui::Text("None");
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
	if (!_currentMesh)
		return;
	auto MVP = App::GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	GetMesh()->Update(MVP);
}