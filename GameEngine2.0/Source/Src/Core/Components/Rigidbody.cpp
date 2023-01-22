#include "Include/Core/Components/Rigidbody.h"
#include "Include/App.h"
#include "Include/Resources/Texture.h"

Core::Components::Rigidbody::Rigidbody()
{
	this->ComponentName = "Rigidbody";
}

Core::Components::Rigidbody::~Rigidbody()
{
}

void Core::Components::Rigidbody::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/RigidbodyIcon.png");
}
