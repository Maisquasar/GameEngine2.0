#include "Include/EditorUi/MainBar.h"
#include "Include/App.h"
#include "Include/EditorUi/Editor.h"

EditorUi::MainBar::MainBar()
{
}

EditorUi::MainBar::~MainBar()
{
}

void EditorUi::MainBar::Draw()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl+N"))
			{
				//TODO: New Scene + shortcut
			}
			if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
			{
				//TODO: Open Scene via Explorer + shortcut.
			}
			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				//TODO: Save Scene + shortcut.
			}
			if (ImGui::MenuItem("Save as Scene", "Ctrl+Shift+S"))
			{
				//TODO: Save Scene via Explorer + shortcut.
				EditorUi::Editor::GetFileExplorer()->SetOpen(true);
			}
			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				App::CloseApp();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}