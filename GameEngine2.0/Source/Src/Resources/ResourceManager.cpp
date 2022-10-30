#include "Include/Resources/ResourceManager.h"
#include "Include/Core/Components/Mesh.h"
std::unordered_map<std::string, Resources::IResource*> Resources::ResourceManager::_resource;

Resources::ResourceManager::ResourceManager() {}

Resources::ResourceManager::~ResourceManager() 
{
	for (auto i : _resource)
	{
		delete i.second;
	}
	_resource.clear();
}

Core::Components::Mesh* Resources::ResourceManager::MeshPopup()
{
	if (ImGui::BeginPopup("MeshPopup")) {
		for (auto resource : _resource)
		{
			if (auto mesh = dynamic_cast<Core::Components::Mesh*>(resource.second))
			{
				ImGui::Button(mesh->GetName().c_str());
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}
	return nullptr;
}

