#include "..\..\..\Include\Core\Components\CameraComponent.h"
#include "Include/Render/EditorIcon.h"
#include "Include/Render/InstancesManager.h"
#include "Include/App.h"
#include "Include/Core/Node.h"


Core::Components::CameraComponent::CameraComponent()
{
	ComponentName = "Camera";
}

Core::Components::CameraComponent::~CameraComponent()
{
	delete Icon;
}

void Core::Components::CameraComponent::Initialize()
{
}

void Core::Components::CameraComponent::Update()
{
	if (!Icon)
	{
		Icon = new Render::EditorIcon();
		Icon->Initialize("CameraMat");
		Icon->SetSize(Math::Vec2(0.25f, 0.25f));

	}
	Icon->Draw(Application.GetScene()->GetVPMatrix(), GameObject->Transform, GameObject->IsSelected());
}

void Core::Components::CameraComponent::DrawPicking(int id)
{
	Icon->DrawPicking(Application.GetScene()->GetVPMatrix(), GameObject->Transform, id);
}

void Core::Components::CameraComponent::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/CameraIcon.png");
}
