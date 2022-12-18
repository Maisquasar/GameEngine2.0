#include "Include/Core/Components/SkeletalMesh.h"
#include "Include/Resources/Skeleton.h"
#include "Include/App.h"
#include "Include/Core/Components/AnimationComponent.h"

Core::Components::SkeletalMesh::SkeletalMesh() { this->ComponentName = "SkeletalMesh"; }

Core::Components::SkeletalMesh::~SkeletalMesh()
{
	if (GameObject)
		if (auto anim = GameObject->GetComponent<AnimationComponent>())
			anim->SetSkeleton(nullptr);
	delete Mesh;
}

void Core::Components::SkeletalMesh::Update()
{
	if (Skeleton && _showSkeleton) {
		Skeleton->RootBone->DrawDebug();
	}
	if (Mesh && _showMesh) {
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
		SetSkeleton(skel);
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
		if (Mesh)
			delete Mesh;
		Mesh = Cast(Resources::Mesh, m->Clone());
	}
	if (Skeleton)
		Skeleton->RootBone->ShowInInspector();


	ImGui::Checkbox("Show Mesh", &_showMesh);
	ImGui::Checkbox("Show Skeleton", &_showSkeleton);
}

void Core::Components::SkeletalMesh::SetSkeleton(Resources::Skeleton* skel)
{
	Skeleton = skel;
	Skeleton->RootBone->SetParent(GameObject);
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
				Skeleton->RootBone->Transform.Parent = GameObject;
				Skeleton->RootBone->Parent = GameObject;
			}
		}
		pos++;
	}
}
