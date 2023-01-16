#include "..\..\..\Include\Core\Components\Camera.h"
#include "Include/Render/EditorIcon.h"
#include "Include/Render/InstancesManager.h"
#include "Include/App.h"
#include "Include/Core/Node.h"


Core::Components::Camera::Camera()
{
	ComponentName = "Camera";
}

Core::Components::Camera::~Camera()
{
	delete Icon;
}

void Core::Components::Camera::Initialize()
{
}

void Core::Components::Camera::Update()
{
	if (!Icon)
	{
		Icon = new Render::EditorIcon();
		Icon->Initialize("CameraMat");
		Icon->SetSize(Math::Vec2(0.25f, 0.25f));

	}
	Icon->Draw(Application.GetScene()->GetVPMatrix(), GameObject->Transform, GameObject->IsSelected());
}

void Core::Components::Camera::DrawPicking(int id)
{
	Icon->DrawPicking(Application.GetScene()->GetVPMatrix(), GameObject->Transform, id);
}

void Core::Components::Camera::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/CameraIcon.png");
}
