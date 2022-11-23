#include "Include/Core/Components/MeshComponent.h"
#include "Include/Resources/ResourceManager.h"

Core::Components::MeshComponent::MeshComponent()
{
	ComponentName = "MeshComponent";
}

Core::Components::MeshComponent::~MeshComponent()
{
}

void Core::Components::MeshComponent::ShowInInspector()
{
	if (ImGui::Button("Change Mesh"))
	{
		ImGui::OpenPopup("MeshPopup");
	}
	Resources::ResourceManager::MeshPopup();
}
