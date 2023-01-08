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
	delete _instancesManager;
}

void Core::Components::Camera::Initialize()
{
	auto mesh = Application.GetResourceManager()->Get<Resources::Mesh>("DefaultPlane");
	_instancesManager = new Render::InstancesManager();
	_instancesManager->SetInstances((Resources::MeshInstance*)mesh, 10000);
	_instancesManager->SetShader(Application.GetResourceManager()->Get<Resources::Shader>("Assets/Default/Shaders/UnlitInstanceShader"));
	_instancesManager->Initialize();
}

void Core::Components::Camera::Update()
{
	if (!Icon)
	{
		Icon = new Render::EditorIcon();
		Icon->Initialize("CameraMat");
		Icon->SetSize(Math::Vector2(0.25f, 0.25f));

	}
	Icon->Draw(Application.GetScene()->GetVPMatrix(), GameObject->Transform);

	_instancesManager->Draw();
}

void Core::Components::Camera::DrawPicking(int id)
{
	Icon->DrawPicking(Application.GetScene()->GetVPMatrix(), GameObject->Transform, id);
}
