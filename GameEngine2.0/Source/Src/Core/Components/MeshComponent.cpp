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
	if (auto Mesh = Resources::ResourceManager::ResourcesPopup<Resources::Mesh>("MeshPopup")) {
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
		ImGui::PushID(index++);
		ImGui::Separator();
		if (ImGui::CollapsingHeader(Sub.Material->GetName().c_str())) {

			ImGui::BeginDisabled(!Sub.Material->IsEditable());

			// Shader
			if (ImGui::Button("Shader"))
			{
				ImGui::OpenPopup("ShaderPopup");
			}
			if (auto sha = Resources::ResourceManager::ResourcesPopup<Resources::Shader>("ShaderPopup"))
				Sub.Material->SetShader(sha);
			ImGui::SameLine();
			ImGui::Text("%s", Sub.Material->GetShader()->GetName().c_str());

			// Texture
			if (ImGui::Button("Change Texture"))
			{
				ImGui::OpenPopup("TexturePopup");
			}
			if (auto tex = Resources::ResourceManager::ResourcesPopup<Resources::Texture>("TexturePopup"))
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
	if (!_currentMesh)
		return;
	auto MVP = App::GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	if (_currentMesh)
		GetMesh()->Update(MVP);
}

void Core::Components::MeshComponent::Save(std::string space, std::string& content)
{
	content += space + Utils::Loader::StringFormat("Mesh : %s\n", GetMesh()->GetPath().c_str());
	int index = 0;
	for (auto Sub : GetMesh()->SubMeshes)
	{
		content += space + Utils::Loader::StringFormat("SubMesh[%d] : %s\n", index++, Sub.Material->GetPath().c_str());
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
			if (auto mesh = Resources::ResourceManager::Get<Resources::Mesh>(MeshPath.c_str())) {
				this->_currentMesh = mesh->Clone();
			}
			else
			{
				_currentMesh = new Resources::Mesh();
				_currentMesh->SetPath(MeshPath);
				App::MultiThreadMeshes.push_back(&_currentMesh);
			}
		}
		else if (currentLine.substr(0, 7) == "SubMesh")
		{
			auto SubMeshMaterialPath = Utils::Loader::GetString(currentLine);
			if (auto mat = Resources::ResourceManager::Get<Resources::Material>(SubMeshMaterialPath.c_str())) {
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
