#include "Include/Resources/ResourceManager.h"

std::unordered_map<std::string, Resources::IResource*> Resources::ResourceManager::_resource;
int Resources::ResourceManager::TextureIndex = 0;

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

