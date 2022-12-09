#include "Include/Resources/ResourceManager.h"

Resources::ResourceManager::ResourceManager() {}

Resources::ResourceManager::~ResourceManager() 
{
	for (auto i : _resource)
	{
		delete i.second;
	}
	_resource.clear();
}

void Resources::ResourceManager::RecompileShaders()
{
	for (auto resources : _resource)
	{
		if (auto Shader = dynamic_cast<Resources::Shader*>(resources.second))
		{
			PrintLog("Recompiling Shader %s", Shader->GetPath().c_str());
			Shader->Load("");
		}
	}
}

Resources::Shader* Resources::ResourceManager::GetDefaultShader()
{
	return Get<Resources::Shader>("Assets/Default/Shaders/UnlitShader");
}

Resources::Shader* Resources::ResourceManager::GetPickingShader()
{
	return Get<Resources::Shader>("Assets/Default/Shaders/PickingShader");
}

Resources::Mesh* Resources::ResourceManager::GetDefaultCube()
{
	return Get<Resources::Mesh>("Cube.obj::Cube");
}


