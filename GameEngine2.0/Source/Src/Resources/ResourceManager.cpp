#include "Include/Resources/ResourceManager.h"

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

