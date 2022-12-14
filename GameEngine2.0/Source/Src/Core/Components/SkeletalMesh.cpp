#include "Include/Core/Components/SkeletalMesh.h"
#include "Include/Resources/Skeleton.h"
#include "Include/App.h"

Core::Components::SkeletalMesh::SkeletalMesh() { this->ComponentName = "SkeletalMesh"; }

Core::Components::SkeletalMesh::~SkeletalMesh() { delete Mesh; }

void Core::Components::SkeletalMesh::Update()
{
	if (Skeleton) {
		Skeleton->RootBone->DrawDebug();
	}
	if (Mesh) {
		Mesh->Update(Application.GetVPMatrix() * GameObject->Transform.GetModelMatrix());
	}
}

void Core::Components::SkeletalMesh::ShowInInspector()
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
		Skeleton = skel;
		Skeleton->RootBone->SetParent(GameObject);
	}

	// Mesh Button
	if (ImGui::Button("Mesh"))
	{
		ImGui::OpenPopup("MeshPopup");
	}
	ImGui::SameLine();
	name = "None";
	if (Mesh)
	{
		name = Mesh->GetName();
	}
	ImGui::Text(name.c_str());
	if (auto m = Application.GetResourceManager()->ResourcesPopup<Resources::Mesh>("MeshPopup")) {
		Mesh = Cast(Resources::Mesh, m->Clone());
	}
}

void Core::Components::SkeletalMesh::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Skeleton : %s\n", this->Skeleton->GetPath().c_str());
	content += space + Utils::StringFormat("Mesh : %s\n", this->Mesh->GetPath().c_str());
}

void Core::Components::SkeletalMesh::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 4) == "Mesh")
		{
			auto MeshPath = Utils::Loader::GetString(currentLine);
			if (auto mesh = Application.GetResourceManager()->Get<Resources::Mesh>(MeshPath.c_str())) {
				Mesh = Cast(Resources::Mesh, mesh->Clone());
			}
		}
		else if (currentLine.substr(0, 8) == "Skeleton")
		{
			auto SkelPath = Utils::Loader::GetString(currentLine);
			if (auto skel = Application.GetResourceManager()->Get<Resources::Skeleton>(SkelPath.c_str())) {
				Skeleton = skel;
			}
		}
		pos++;
	}
}
