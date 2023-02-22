#include "stdafx.h"
#include "Include/Core/Components/SkeletalMeshComponent.h"
#include "Include/Resources/Skeleton.h"
#include "Include/Resources/SkeletalMesh.h"
#include "Include/Resources/Mesh.h"
#include "Include/App.h"
#include "Include/Core/Components/AnimationComponent.h"
#include "Include/Utils/Utils.h"
#include "Include/Utils/Loader.h"
#include "Include/EditorUi/Inspector.h"

Core::Components::SkeletalMeshComponent::SkeletalMeshComponent() { this->ComponentName = "SkeletalMeshComponent"; }

Core::Components::SkeletalMeshComponent::~SkeletalMeshComponent()
{
	if (GameObject)
		if (auto anim = GameObject->GetComponent<AnimationComponent>())
			anim->SetSkeleton(nullptr);
	if (Skeleton)
		delete Skeleton;
}

void Core::Components::SkeletalMeshComponent::DrawPicking(int id)
{
	if (!Mesh || !_enable)
		return;
	if (Mesh && _showMesh)
		Mesh->DrawPicking(this->GameObject->Transform.GetModelMatrix(), _materials, Skeleton, id);
}

void Core::Components::SkeletalMeshComponent::Draw()
{
	if (Skeleton && _showSkeleton && Skeleton->RootBone) {
		Skeleton->RootBone->DrawDebug();
	}
	if (Mesh && _showMesh) {
		Mesh->Update(GameObject->Transform.GetModelMatrix(), _materials, Skeleton, EditorUi::Editor::GetInspector()->IsSelected(GameObject));
	}
}

void Core::Components::SkeletalMeshComponent::Update()
{
}

void Core::Components::SkeletalMeshComponent::ShowInInspector()
{
	// Skeleton Button
	if (ImGui::Button("Skeleton"))
	{
		ImGui::OpenPopup("SkeletonPopup");
	}
	if (auto skel = Application.GetResourceManager()->ResourcesPopup<Resources::Skeleton>("SkeletonPopup")) {
		SetSkeleton(skel);
	}
	ImGui::SameLine();
	std::string name = "None";
	if (Skeleton)
	{
		name = Skeleton->GetName();
	}
	ImGui::Text(name.c_str());
	ImGui::SameLine();
	if (Skeleton)
	{
		ImGui::Text("| Bones %d", Skeleton->BoneCount);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset") && Skeleton && Skeleton->RootBone)
	{
		Skeleton->RootBone->RemoveFromParent();
		delete Skeleton;
		Skeleton = nullptr;
	}

	// Mesh Button
	if (ImGui::Button("Mesh"))
	{
		ImGui::OpenPopup("SkeletalMeshPopup");
	}
	if (auto m = Application.GetResourceManager()->ResourcesPopup<Resources::SkeletalMesh>("SkeletalMeshPopup")) {
		SetMesh(m);
	}
	ImGui::SameLine();
	name = "None";
	if (Mesh)
	{
		name = Mesh->GetName();
	}
	ImGui::Text(name.c_str());
	if (Skeleton)
		Skeleton->RootBone->ShowInInspector();

	if (Mesh)
	{
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


	ImGui::Checkbox("Show Mesh", &_showMesh);
	ImGui::Checkbox("Show Skeleton", &_showSkeleton);
}

void Core::Components::SkeletalMeshComponent::SetSkeleton(Resources::Skeleton* skel)
{
	if (!skel)
		return;
	if (Skeleton && Skeleton != skel)
		delete Skeleton;
	if (!Application.GetResourceManager()->IsDeleted()) {
		Skeleton = dynamic_cast<Resources::Skeleton*>(skel->Clone());
		Skeleton->RootBone->SetParent(GameObject);
	}
}

void Core::Components::SkeletalMeshComponent::SetMesh(Resources::Mesh* mesh)
{
	if (!mesh)
		return;
	Mesh = (Resources::SkeletalMesh*)mesh;
	_materials.resize(mesh->SubMeshes.size());
	for (auto& mat : _materials)
	{
		if (!mat)
		{
			mat = Application.GetResourceManager()->Get<Resources::Material>("DefaultMaterial");
		}
	}
}

void Core::Components::SkeletalMeshComponent::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Skeleton : %s\n", Skeleton ? Skeleton->GetPath().c_str() : "None");
	content += space + Utils::StringFormat("Mesh : %s\n", Mesh ? Mesh->GetPath().c_str() : "None");
}

void Core::Components::SkeletalMeshComponent::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	Skeleton = new Resources::Skeleton();
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 4) == "Mesh")
		{
			auto MeshPath = Utils::Loader::GetString(currentLine);
			if (auto mesh = Application.GetResourceManager()->Get<Resources::Mesh>(MeshPath.c_str())) {
				Mesh = Cast(Resources::SkeletalMesh, mesh);
			}
		}
		else if (currentLine.substr(0, 8) == "Skeleton")
		{
			auto SkelPath = Utils::Loader::GetString(currentLine);
			Skeleton->SetPath(SkelPath);
			auto name = SkelPath.substr(SkelPath.find_first_of(':') + 2);
			Skeleton->SetName(name);
		}
		pos++;
	}
}

Core::Components::SkeletalMeshComponent* Core::Components::SkeletalMeshComponent::Clone() const
{
	auto cloned = new SkeletalMeshComponent(static_cast<SkeletalMeshComponent const&>(*this));
	cloned->SetSkeleton(this->Skeleton);
	return cloned;
}

void Core::Components::SkeletalMeshComponent::AddMaterial(Resources::Material* mat)
{
	this->_materials.push_back(mat);
}

void Core::Components::SkeletalMeshComponent::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/SkeletalMeshIcon.png");
}
