#include "Include/Core/Components/MeshComponent.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/Resources/Mesh.h"
#include "Include/Resources/Material.h"
#include "Include/Core/Node.h"
#include "Include/Utils/Utils.h"
#include "Include/Utils/Loader.h"
#include "Include/Render/EditorIcon.h"

Core::Components::MeshComponent::MeshComponent()
{
	ComponentName = "MeshComponent";
}

Core::Components::MeshComponent::~MeshComponent()
{
}

float getDistance(Math::Vec3 point1, Math::Vec3 point2, Math::Vec3 axis)
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
	if (_currentMesh)
		ImGui::TextUnformatted(_currentMesh->GetName().c_str());
	else
		ImGui::TextUnformatted("None");
	if (auto Mesh = Application.GetResourceManager()->ResourcesPopup<Resources::Mesh>("MeshPopup")) {
		SetMesh(Mesh);
	}
	if (!_currentMesh)
		return;

	// Material List.
	static int SelectedRow = 0;
	if (ImGui::BeginTable("Materials", 3, ImGuiTableFlags_Borders))
	{
		for (int row = 0; row < _materials.size(); row++)
		{
			ImGui::TableNextRow();
			for (int column = 0; column < 3; column++)
			{
				ImGui::TableSetColumnIndex(column);
				switch (column)
				{
				case 0:
					ImGui::TextUnformatted(std::to_string(row).c_str());
					break;
				case 1:
					if (_materials[row])
						ImGui::TextUnformatted(_materials[row]->GetName().c_str());
					else
						ImGui::TextUnformatted("Missing Material");
					break;
				case 2:
					ImGui::PushID(row);
					if (ImGui::Button("Change Material"))
					{
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
			_materials[SelectedRow] = mat;
		}
		ImGui::PopID();
		ImGui::EndTable();
	}

	// Material Components List.
	int index = 0;
	for (auto mat : _materials)
	{
		if (!mat)
			continue;
		ImGui::PushID(index++);
		ImGui::Separator();
		if (ImGui::CollapsingHeader(mat->GetName().c_str())) {

			ImGui::BeginDisabled(!mat->IsEditable());

			// Shader
			if (ImGui::Button("Shader"))
			{
				ImGui::OpenPopup("ShaderPopup");
			}
			if (auto sha = Application.GetResourceManager()->ResourcesPopup<Resources::Shader>("ShaderPopup"))
				mat->SetShader(sha);
			ImGui::SameLine();
			ImGui::Text("%s", mat->GetShader()->GetName().c_str());

			// Texture
			if (ImGui::Button("Change Texture"))
			{
				ImGui::OpenPopup("TexturePopup");
			}
			if (auto tex = Application.GetResourceManager()->TexturePopup("TexturePopup"))
				mat->SetTexture(tex);
			ImGui::SameLine();
			if (mat->GetTexture())
			{
				ImGui::Text(mat->GetTexture()->GetName().c_str());
			}
			else
				ImGui::Text("None");
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
			{
				mat->SetTexture(nullptr);
			}

			// Parameters
			ImGui::ColorEdit4("Ambient", &(*mat->GetPtrAmbient())[0]);
			ImGui::ColorEdit4("Diffuse", &(*mat->GetPtrDiffuse())[0]);
			ImGui::ColorEdit4("Specular", &(*mat->GetPtrSpecular())[0]);
			ImGui::EndDisabled();
		}
		ImGui::PopID();
	}
}

void Core::Components::MeshComponent::Update()
{
}

void Core::Components::MeshComponent::Draw()
{
	if (!_currentMesh || !_enable)
		return;
	auto MVP = Application.GetScene()->GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	if (_currentMesh)
		GetMesh()->Update(MVP, _materials, EditorUi::Editor::GetInspector()->IsSelected(GameObject));
}

void Core::Components::MeshComponent::DrawPicking(int id)
{
	if (!_currentMesh || !_enable)
		return;
	auto MVP = Application.GetScene()->GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	if (_currentMesh)
		GetMesh()->DrawPicking(MVP, _materials, id);
}

Resources::Mesh* Core::Components::MeshComponent::GetMesh()
{
	return static_cast<Resources::Mesh*>(_currentMesh);
}

void Core::Components::MeshComponent::SetMesh(Resources::IResource* mesh)
{
	_currentMesh = mesh;
	_materials.resize((dynamic_cast<Resources::Mesh*>(_currentMesh))->SubMeshes.size());
	for (auto& mat : _materials)
	{
		if (!mat)
		{
			mat = Application.GetResourceManager()->Get<Resources::Material>("DefaultMaterial");
		}
	}
}

void Core::Components::MeshComponent::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Mesh : %s\n", GetMesh()->GetPath().c_str());
	int index = 0;
	for (auto mat : _materials)
	{
		content += space + Utils::StringFormat("SubMesh[%d] : %s\n", index++, mat->GetPath().c_str());
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
				this->_currentMesh = mesh;
			}
			else
			{
				_currentMesh = new Resources::Mesh();
				_currentMesh->SetPath(MeshPath);
#if MULTITHREAD_LOADING
				//Application.MultiThreadMeshes.push_back(&_currentMesh);
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
				_materials.push_back(mat);
				SubMeshIndex++;
			}
		}
		pos++;
	}
}

void Core::Components::MeshComponent::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/MeshIcon.png");
}

void Core::Components::MeshComponent::AddMaterial(Resources::Material* mat)
{
	this->_materials.push_back(mat);
}
