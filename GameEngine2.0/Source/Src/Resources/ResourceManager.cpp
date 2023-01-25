#include "Include/Resources/ResourceManager.h"

#include "Include/Utils/Utils.h"

Resources::ResourceManager::ResourceManager() { TextureIndex = 0; }

Resources::ResourceManager::~ResourceManager()
{
	_deleted = true;
	for (auto it = _resource.begin(); it != _resource.end(); )
	{
		delete it->second;
		it = _resource.erase(it);
	}
	_resource.clear();
}

void Resources::ResourceManager::RemoveResource(Resources::IResource* resource)
{
	_resource.erase(resource->GetPath());
}

void Resources::ResourceManager::LoadDefaultsResources()
{

	auto billboardPicking = new Resources::Shader();

	billboardPicking->SetPath("Assets/Default/Shaders/BillboardPickingShader");
	billboardPicking->SetName("BillboardPickingShader");

	auto billboardShad = GetBillboardShader();
	auto pickingShad = GetPickingShader();

	billboardPicking->SetShader(billboardShad->VertexPath);
	billboardPicking->SetFragmentShader(pickingShad->FragmentPath);

	billboardPicking->Link();
	billboardPicking->Initialize();

	Add(billboardPicking->GetPath(), billboardPicking);


	auto billboardInstancePicking = new Resources::Shader();

	billboardInstancePicking->SetPath("Assets/Default/Shaders/BillboardPickingInstanceShader");
	billboardInstancePicking->SetName("BillboardPickingShader");

	auto billboardInstanceShad = Get<Resources::Shader>("Assets/Default/Shaders/BillboardInstanceShader");

	billboardInstancePicking->SetShader(billboardInstanceShad->VertexPath);
	billboardInstancePicking->SetFragmentShader(pickingShad->FragmentPath);

	billboardInstancePicking->Link();
	billboardInstancePicking->Initialize();

	Add(billboardInstancePicking->GetPath(), billboardInstancePicking);
}

void Resources::ResourceManager::RecompileShaders()
{
	for (auto resources : _resource)
	{
		if (auto Shader = dynamic_cast<Resources::Shader*>(resources.second))
		{
			PrintLog("Recompiling Shader %s", Shader->GetPath().c_str());
			Shader->Recompile();
		}
	}
}

Resources::PhysicMaterial* Resources::ResourceManager::GetDefaultPhysicMaterial()
{
	return Get<Resources::PhysicMaterial>("Assets/Default/Physic/Default.phm");
}

Resources::Shader* Resources::ResourceManager::GetDefaultShader()
{
	return Get<Resources::Shader>("Assets/Default/Shaders/UnlitShader");
}

Resources::Shader* Resources::ResourceManager::GetDefaultAnimShader()
{
	return Get<Resources::Shader>("Assets/Default/Shaders/UnlitAnimShader");
}

Resources::Shader* Resources::ResourceManager::GetPickingShader()
{
	return Get<Resources::Shader>("Assets/Default/Shaders/PickingShader");
}

Resources::Shader* Resources::ResourceManager::GetPickingAnimShader()
{
	return Get<Resources::Shader>("Assets/Default/Shaders/PickingAnimShader");
}

Resources::Shader* Resources::ResourceManager::GetBillboardShader()
{
	return Get<Resources::Shader>("Assets/Default/Shaders/BillboardShader");
}

Resources::Shader* Resources::ResourceManager::GetBillboardPickingShader()
{
	return Get<Resources::Shader>("Assets/Default/Shaders/BillboardPickingShader");
}

Resources::Mesh* Resources::ResourceManager::GetDefaultCube()
{
	return Get<Resources::Mesh>("Cube.obj::Cube");
}

Resources::Mesh* Resources::ResourceManager::GetDefaultPlane()
{
	return Get<Resources::Mesh>("DefaultPlane");
}

Resources::Texture* Resources::ResourceManager::TexturePopup(const char* popupName)
{
	Resources::Texture* out = nullptr;
	int id = 0;
	if (ImGui::BeginPopupModal(popupName, (bool*)0, ImGuiWindowFlags_AlwaysAutoResize)) {
		static ImGuiTextFilter filter;
		filter.Draw();
		for (auto resource : _resource)
		{
			if (auto res = dynamic_cast<Resources::Texture*>(resource.second))
			{
				if (!filter.PassFilter(res->GetName().c_str()))
					continue;
				ImGui::PushID(id++);
				ImGui::Image((ImTextureID)static_cast<uintptr_t>(res->GetData()), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::Selectable(res->GetName().c_str(), false, ImGuiSelectableFlags_SpanAllColumns)) {
					out = res;
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::TextUnformatted(res->GetPath().c_str());
					ImGui::EndTooltip();
				}
				ImGui::PopID();
			}
		}
		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return out;
}

