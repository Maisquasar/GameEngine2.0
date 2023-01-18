#include "..\..\..\Include\Core\Components\CameraComponent.h"
#include "Include/Render/EditorIcon.h"
#include "Include/Render/InstancesManager.h"
#include "Include/App.h"
#include "Include/Core/Node.h"
#include "Include/Utils/Loader.h"
#include "Include/EditorUi/GameWindow.h"


Core::Components::CameraComponent::CameraComponent()
{
	ComponentName = "Camera";
}

Core::Components::CameraComponent::~CameraComponent()
{
	if (this->FrameBuffer.Tex)
	{
		Application.GetResourceManager()->RemoveResource(FrameBuffer.Tex);
		delete FrameBuffer.Tex;
	}
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
	if (Application.GetScene()->GetMainCamera() == this)
	{
		Application.GetScene()->SetMainCamera(nullptr);
	}
	delete Icon;
}

void Core::Components::CameraComponent::Initialize()
{
	Application.GetScene()->Cameras.push_back(this);
	FrameBuffer.Initialize(Math::Vec2(1920, 1080));
	if (!Application.GetScene()->GetMainCamera())
	{
		SetMainCamera();
	}
	
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
	if (ImGui::Begin("Camera Preview", (bool*)true, ImGuiWindowFlags_NoFocusOnAppearing))
	{
		auto size = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)static_cast<uintptr_t>(FrameBuffer.Tex->GetData()), ImVec2(size.x - 16, size.y - 50), ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
	ImGui::BeginDisabled(this->_MainCamera);
	if (ImGui::Button("Set Main Camera")) { SetMainCamera(); }
	ImGui::EndDisabled();

}

void Core::Components::CameraComponent::SetMainCamera()
{
	this->_MainCamera = true;
	Application.GetScene()->SetMainCamera(this);
}

void Core::Components::CameraComponent::SetUIIcon()
{
	this->_UIIcon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/CameraIcon.png");
}

bool Core::Components::CameraComponent::IsVisible()
{
	return GameObject->IsSelected() || (_MainCamera && EditorUi::Editor::GetGameWindow()->Window->DockTabIsVisible);
}

Core::Transform* Core::Components::CameraComponent::GetTransform()
{
	return &this->GameObject->Transform;
}

void Core::Components::CameraComponent::Save(std::string space, std::string& content)
{
	content += space + Utils::StringFormat("MainCamera : %d\n", this->_MainCamera);
}

void Core::Components::CameraComponent::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 13) != "#EndComponent")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 10) == "MainCamera")
		{
			if ((bool)Utils::Loader::GetInt(currentLine))
				SetMainCamera();
		}
		pos++;
	}
}
