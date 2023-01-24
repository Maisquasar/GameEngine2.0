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

void Core::Components::Rigidbody::BeginPause()
{
}

void Core::Components::Rigidbody::ShowInInspector()
{
	ImGui::DragFloat3("Initial Velocity", &_velocity.x, 0.1f);
	ImGui::DragFloat3("Initial Angular Velocity", &_angularVelocity.x, 0.1f);
	ImGui::DragFloat("Mass", &_mass, 0.1f);
	_mass = Math::Max(_mass, 0.01f);
	ImGui::Checkbox("Use Gravity", &_useGravity);
	if (_body) {
		_body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !_useGravity);
		_body->setMass(_mass);
		auto vel = _body->getLinearVelocity();
		if (ImGui::DragFloat3("Velocity", &vel.x, 0.1f))
			_body->setLinearVelocity(vel);
		vel = _body->getAngularVelocity();
		if (ImGui::DragFloat3("Angular Velocity", &vel.x, 0.1f))
			_body->setAngularVelocity(vel);
	}
}

void Core::Components::Rigidbody::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("Mass : %f\n", _mass);
	content += space + Utils::StringFormat("Velocity : %s\n", _velocity.ToString().c_str());
	content += space + Utils::StringFormat("Angular : %s\n", _angularVelocity.ToString().c_str());
	content += space + Utils::StringFormat("Gravity : %d\n", _useGravity);
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
		else if (currentLine.substr(0, 7) == "Angular")
		{
			_angularVelocity = Utils::Loader::GetVector3(currentLine);
		}
		else if (currentLine.substr(0, 7) == "Gravity")
		{
			_useGravity = Utils::Loader::GetInt(currentLine);
		}
		pos++;
	}
}

void Core::Components::Rigidbody::SetParameters(physx::PxRigidDynamic* body)
{
	_body = body;
	body->setMass(_mass);
	body->setLinearVelocity({ _velocity.x, _velocity.y, _velocity.z });
	body->setAngularVelocity({ _angularVelocity.x, _angularVelocity.y,  _angularVelocity.z});
	body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !_useGravity);
	body->setSleepThreshold(0);
}
