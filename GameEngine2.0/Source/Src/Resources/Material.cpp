#include "..\..\Include\Resources\Material.h"
#include "Include/Resources/ResourceManager.h"

Resources::Material::Material()
{
	_shader = Resources::ResourceManager::Get<Resources::Shader>("Assets/Default/Shaders/UnlitShader");
}

Resources::Material::~Material()
{
}
