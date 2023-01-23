#include "Include/Core/Components/Rigidbody.h"
#include "Include/App.h"
#include "Include/Resources/Texture.h"
#include "Include/Utils/Loader.h"

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

void Core::Components::Rigidbody::ShowInInspector()
{
	ImGui::DragFloat("Mass", &_mass, 0.1f);
	ImGui::DragFloat3("Initial Velocity", &_velocity.x, 0.1f);
}

void Core::Components::Rigidbody::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Mass : %f\n", _mass);
	content += space + Utils::StringFormat("Velocity : %s\n", _velocity.ToString().c_str());
}

void Core::Components::Rigidbody::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 4) == "Mass")
		{
			_mass = Utils::Loader::GetFloat(currentLine);
		}
		else if (currentLine.substr(0, 8) == "Velocity")
		{
			_velocity = Utils::Loader::GetVector3(currentLine);
		}
		pos++;
	}
}
