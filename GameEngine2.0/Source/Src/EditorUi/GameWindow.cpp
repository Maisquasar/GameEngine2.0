#include "..\..\Include\EditorUi\GameWindow.h"
#include "Include\App.h"

EditorUi::GameWindow::GameWindow()
{
}

EditorUi::GameWindow::~GameWindow()
{
}

void EditorUi::GameWindow::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Game", &_open))
	{
		if (!Window)
			Window = ImGui::GetCurrentWindow();
		auto size = ImGui::GetWindowSize();
		if (Application.GetScene()->GetMainCamera())
			ImGui::Image((ImTextureID)static_cast<uintptr_t>(Application.GetScene()->GetMainCamera()->FrameBuffer.Tex->GetData()), ImVec2(size.x - 16, size.y - 60), ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
}
