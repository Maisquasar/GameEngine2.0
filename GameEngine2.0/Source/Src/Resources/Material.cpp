#include "..\..\Include\Resources\Material.h"
#include "Include/Resources/ResourceManager.h"

Resources::Material::Material()
{
	_shader = Resources::ResourceManager::Get<Resources::Shader>("UnlitShader");
}

Resources::Material::~Material()
{
}
