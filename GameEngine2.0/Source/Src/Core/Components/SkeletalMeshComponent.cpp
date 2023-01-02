#include "Include/Core/Components/SkeletalMeshComponent.h"
#include "Include/Resources/Skeleton.h"
#include "Include/Resources/SkeletalMesh.h"
#include "Include/App.h"
#include "Include/Core/Components/AnimationComponent.h"
#include "Include/Utils/Utils.h"
#include "Include/Utils/Loader.h"

Core::Components::SkeletalMeshComponent::SkeletalMeshComponent() { this->ComponentName = "SkeletalMeshComponent"; }

Core::Components::SkeletalMeshComponent::~SkeletalMeshComponent()
{
	if (GameObject)
		if (auto anim = GameObject->GetComponent<AnimationComponent>())
			anim->SetSkeleton(nullptr);
	delete Mesh;
	delete Skeleton;
}

void Core::Components::SkeletalMeshComponent::DrawPicking(int id)
{
	if (!Mesh || !_enable)
		return;
	auto MVP = Application.GetScene()->GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	if (Mesh)
		Mesh->DrawPicking(MVP, id);
}

#include "Include/EditorUi/Inspector.h"
void Core::Components::SkeletalMeshComponent::Update()
{
	if (Skeleton && _showSkeleton && Skeleton->RootBone) {
		Skeleton->RootBone->DrawDebug();
	}
	if (Mesh && _showMesh) {
		Mesh->Update(GameObject->Transform.GetModelMatrix(), Skeleton, EditorUi::Editor::GetInspector()->IsSelected(GameObject));
	}
}

void Core::Components::SkeletalMeshComponent::ShowInInspector()
{
	// Skeleton Button
	if (ImGui::Button("Skeleton"))
	{
		ImGui::OpenPopup("SkeletonPopup");
	}
	ImGui::SameLine();
	std::string name = "None";
	if (Skeleton)
	{
		name = Skeleton->GetName();
	}
	ImGui::Text(name.c_str());
	if (auto skel = Application.GetResourceManager()->ResourcesPopup<Resources::Skeleton>("SkeletonPopup")) {
		SetSkeleton(skel);
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
	ImGui::SameLine();
	name = "None";
	if (Mesh)
	{
		name = Mesh->GetName();
	}
	ImGui::Text(name.c_str());
	if (auto m = Application.GetResourceManager()->ResourcesPopup<Resources::SkeletalMesh>("SkeletalMeshPopup")) {
		if (Mesh)
			delete Mesh;
		Mesh = Cast(Resources::SkeletalMesh, m->Clone());
		printf(Mesh->GetName().c_str());
	}
	if (Skeleton)
		Skeleton->RootBone->ShowInInspector();


	ImGui::Checkbox("Show Mesh", &_showMesh);
	ImGui::Checkbox("Show Skeleton", &_showSkeleton);
}

void Core::Components::SkeletalMeshComponent::SetSkeleton(Resources::Skeleton* skel)
{
	if (Skeleton)
		delete Skeleton;
	Skeleton = Cast(Resources::Skeleton, skel->Clone());
	Skeleton->RootBone->SetParent(GameObject);
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
				Mesh = Cast(Resources::SkeletalMesh, mesh->Clone());
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
