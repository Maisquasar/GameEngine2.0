#include "stdafx.h"
#include "..\..\Include\EditorUi\SceneWindow.h"
#include "Include/App.h"

EditorUi::SceneWindow::SceneWindow()
{
}

EditorUi::SceneWindow::~SceneWindow()
{
}

void EditorUi::SceneWindow::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Scene", &_open, ImGuiWindowFlags_MenuBar))
	{
		if (!Window)
			Window = ImGui::GetCurrentWindow();

		if (ImGui::BeginMenuBar()) {
			ImGui::SetNextItemWidth(100);
			ImGui::Combo("##", (int*)&Application.GetSettings()->S_Transform, "Local\0World\0");
			ImGui::SameLine();
			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::Checkbox("Show Grid", &Application.GetScene()->_showGrid);
				ImGui::Checkbox("Show Colliders", &Application.GetSettings()->ShowCollider);
				if (ImGui::Button(Application.GetScene()->GetCameraEditor()->FrameBuffer.shader->GetName().c_str()))
				{
					ImGui::OpenPopup("ScreenShaderPopup");
				}
				if (auto shad = Application.GetResourceManager()->ResourcesPopup<Resources::Shader>("ScreenShaderPopup"))
				{
					Application.GetScene()->GetCameraEditor()->FrameBuffer.shader = shad;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		auto size = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)static_cast<uintptr_t>(Application.GetScene()->GetCameraEditor()->FrameBuffer.Tex->GetData()), ImVec2(size.x - 16, size.y - 60), ImVec2(0, 1), ImVec2(1, 0));
		_size = Math::Vec2(size.x - 16, size.y - 60);
		IsHovered = ImGui::IsItemHovered();
		IsFocused = ImGui::IsWindowFocused();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
			UpdateCameraEditor = true;
		if (UpdateCameraEditor && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
			UpdateCameraEditor = false;
	}
	ImGui::End();
}

Math::Vec2 EditorUi::SceneWindow::GetSize()
{
	return _size;
}

Math::Vec2 EditorUi::SceneWindow::GetPos()
{
	if (Window)
		return  Math::Vec2(Window->Pos.x + 7, Window->Pos.y + 46);
	return  Math::Vec2(0, 0);
}

Math::Vec2 EditorUi::SceneWindow::GetMousePosition()
{
	auto mouse = ImGui::GetMousePos();
	return Math::Vec2(mouse.x, mouse.y) - GetPos();
}
