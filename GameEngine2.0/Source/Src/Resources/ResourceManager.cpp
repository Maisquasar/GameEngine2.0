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

