#include "Include/Resources/ResourceManager.h"
std::unordered_map<std::string, std::unique_ptr<Resources::IResource>> Resources::ResourceManager::_resource;

Resources::ResourceManager::ResourceManager() {}

Resources::ResourceManager::~ResourceManager() 
{
	_resource.clear();
}

