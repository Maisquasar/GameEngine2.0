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
	for (int i = 0; i < Application.GetScene()->Cameras.size(); i++)
	{
		if (Application.GetScene()->Cameras[i] == this)
		{
			Application.GetScene()->Cameras.erase(Application.GetScene()->Cameras.begin() + i);
		}
	}
	if (Application.GetScene()->GetCurrentCamera() == this)
	{
		Application.GetScene()->SetCurrentCamera(nullptr);
	}
	delete Icon;
}

void Core::Components::CameraComponent::Initialize()
{
	Application.GetScene()->Cameras.push_back(this);
	FrameBuffer.Initialize(Math::Vec2(1920, 1080));
	
}

void Core::Components::CameraComponent::Update()
{
}

void Core::Components::CameraComponent::EditorUpdate()
{
	if (!Icon)
	{
		Icon = new Render::EditorIcon();
		Icon->Initialize("CameraMat");
		Icon->SetSize(Math::Vec2(0.25f, 0.25f));

	}
}

void Core::Components::CameraComponent::EditorDraw()
{
	if (Icon)
		Icon->Draw(Application.GetScene()->GetVPMatrix(), GameObject->Transform, GameObject->IsSelected());
}

void Core::Components::CameraComponent::DrawPicking(int id)
{
	Icon->DrawPicking(Application.GetScene()->GetVPMatrix(), GameObject->Transform, id);
}

void Core::Components::CameraComponent::ShowInInspector()
{
	ImGui::Begin("Camera Preview", (bool*)true);
	auto size = ImGui::GetWindowSize();
	ImGui::Image((ImTextureID)static_cast<uintptr_t>(FrameBuffer.Tex->GetData()), ImVec2(size.x - 16, size.y - 50), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void Core::Components::CameraComponent::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/CameraIcon.png");
}

bool Core::Components::CameraComponent::IsVisible()
{
	return GameObject->IsSelected();
}

Core::Transform* Core::Components::CameraComponent::GetTransform()
{
	return &this->GameObject->Transform;
}
