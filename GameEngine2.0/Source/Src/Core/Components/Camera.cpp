#include "..\..\..\Include\Core\Components\Camera.h"
#include "Include/Render/EditorIcon.h"
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
	}
	Icon->Draw(Application.GetScene()->GetVPMatrix(), GameObject->Transform);
}

void Core::Components::Camera::DrawPicking(int id)
{
	Icon->DrawPicking(Application.GetScene()->GetVPMatrix(), GameObject->Transform, id);
}
