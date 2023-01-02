#include "Include/Resources/ResourceManager.h"

#include "Include/Utils/Utils.h"

Resources::ResourceManager::ResourceManager() { TextureIndex = 0; }

Resources::ResourceManager::~ResourceManager() 
{
	for (auto it = _resource.begin(); it != _resource.end(); )
	{
		delete it->second;
		it = _resource.erase(it);
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

Resources::Mesh* Resources::ResourceManager::GetDefaultCube()
{
	return Get<Resources::Mesh>("Cube.obj::Cube");
}

Resources::Mesh* Resources::ResourceManager::GetDefaultPlane()
{
	return Get<Resources::Mesh>("DefaultPlane");
}

