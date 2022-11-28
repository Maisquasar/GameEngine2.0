#include "Include/EditorUi/MainBar.h"
#include "Include/App.h"
#include "Include/EditorUi/Editor.h"
#include "Include/Core/Node.h"

EditorUi::MainBar::MainBar()
{
}

EditorUi::MainBar::~MainBar()
{
}

void EditorUi::MainBar::Draw()
{
	static std::string content;
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
				EditorUi::Editor::GetFloatingFileExplorer()->SetOpen(true);
				EditorUi::Editor::GetFloatingFileExplorer()->SetState(EditorUi::FileExplorerState::Read);
			}
			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				App::SaveScene("DefaultScene");
			}
			if (ImGui::MenuItem("Save as Scene", "Ctrl+Shift+S"))
			{
				//TODO: Save Scene via Explorer + shortcut.
				EditorUi::Editor::GetFloatingFileExplorer()->SetOpen(true);
				EditorUi::Editor::GetFloatingFileExplorer()->SetState(EditorUi::FileExplorerState::Write);
				EditorUi::Editor::GetFloatingFileExplorer()->SetTargetExtension(".scene");
				App::SceneNode->Save("", content);
			}
			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				App::CloseApp();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Checkbox("Hierarchy", Editor::GetHierarchy()->IsOpen());
			ImGui::Checkbox("Console", Debug::Log::Console.IsOpen());
			ImGui::Checkbox("File Explorer", Editor::GetFileExplorer()->IsOpen());
			ImGui::Checkbox("Inspector", Editor::GetInspector()->IsOpen());
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
	if (auto file = EditorUi::Editor::GetFloatingFileExplorer()->DrawAndRead())
	{
		App::LoadScene(file->Directory);
		EditorUi::Editor::GetFloatingFileExplorer()->SetOpen(false);
	}
	if (EditorUi::Editor::GetFloatingFileExplorer()->GetState() == EditorUi::FileExplorerState::Write) 
	{
		EditorUi::Editor::GetFloatingFileExplorer()->DrawAndSave(content);
	}
}