#include "Include/EditorUi/MainBar.h"
#include "Include/App.h"
#include "Include/EditorUi/Editor.h"
#include "Include/Core/Node.h"
#include "Include/EditorUi/Editor.h"
#include "Include/EditorUi/Hierarchy.h"
#include "Include/EditorUi/MainBar.h"
#include "Include/EditorUi/FileExplorer.h"
#include "Include/EditorUi/Console.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/EditorUi/ResourcesWindow.h"
#include "Include/EditorUi/PerformanceWindow.h"


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
				EditorUi::Editor::GetFloatingFileExplorer()->SetOpen(true);
				EditorUi::Editor::GetFloatingFileExplorer()->SetState(EditorUi::FileExplorerState::Read);
			}
			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				App::SaveScene();
			}
			if (ImGui::MenuItem("Save as Scene", "Ctrl+Shift+S"))
			{
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
			ImGui::Checkbox("Resources", Editor::GetResourcesWindow()->IsOpen());
			ImGui::Checkbox("Performance", Editor::GetPerformanceWindow()->IsOpen());
			ImGui::EndMenu();
		}
		ImGui::SetCursorPosX((float)(App::GetWindowSize().x / 2) - 25.f);
		if (ImGui::MenuItem("I>"))
		{
			if (App::GetGameState() != GameState::Play)
			{
				App::BeginPlay();
				EditorUi::Editor::GetConsole()->Clear();
			}
		}
		if (ImGui::MenuItem("||"))
		{
			if (App::GetGameState() != GameState::Editor)
			{
				App::EndPlay();
			}
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