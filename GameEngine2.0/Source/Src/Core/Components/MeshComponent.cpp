#include "Include/Core/Components/MeshComponent.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/App.h"

Core::Components::MeshComponent::MeshComponent()
{
	ComponentName = "MeshComponent";
}

Core::Components::MeshComponent::~MeshComponent()
{
}

void Core::Components::MeshComponent::ShowInInspector()
{
	Super::ShowInInspector();
	if (ImGui::Button("Change Mesh"))
	{
		ImGui::OpenPopup("MeshPopup");
	}
	if (auto Mesh = Resources::ResourceManager::MeshPopup())
		_currentMesh = Mesh;
}

void Core::Components::MeshComponent::Update()
{
	if (!_currentMesh)
		return;
	auto MVP = App::GetVPMatrix() * this->GameObject->Transform.GetModelMatrix();
	_currentMesh->Update(MVP);
}
