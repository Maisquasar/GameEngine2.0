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
#include "Include/EditorUi/AnimationWindow.h"


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
				EditorUi::Editor::GetFloatingFileExplorer()->SetTargetExtension(".scene");
			}
			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				Application.GetScene()->SaveScene();
			}
			if (ImGui::MenuItem("Save as Scene", "Ctrl+Shift+S"))
			{
				EditorUi::Editor::GetFloatingFileExplorer()->SetOpen(true);
				EditorUi::Editor::GetFloatingFileExplorer()->SetState(EditorUi::FileExplorerState::Write);
				EditorUi::Editor::GetFloatingFileExplorer()->SetTargetExtension(".scene");
				Application.GetScene()->GetSceneNode()->Save("", content);
			}
			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				Application.CloseApp();
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
			ImGui::Checkbox("Animation", Editor::GetAnimationWindow()->IsOpen());
			ImGui::EndMenu();
		}
		ImGui::SetCursorPosX((float)(Application.GetWindowSize().x / 2) - 25.f);
		if (ImGui::MenuItem("I>"))
		{
			if (Application.GetGameState() != GameState::Play)
			{
				Application.BeginPlay();
				EditorUi::Editor::GetConsole()->Clear();
			}
		}
		if (ImGui::MenuItem("||"))
		{
			if (Application.GetGameState() != GameState::Editor)
			{
				Application.EndPlay();
			}
		}
	}
	ImGui::EndMainMenuBar();
	if (auto file = EditorUi::Editor::GetFloatingFileExplorer()->DrawAndRead())
	{
		Application.GetScene()->LoadScene(file->Directory);
		EditorUi::Editor::GetFloatingFileExplorer()->SetOpen(false);
	}
	if (EditorUi::Editor::GetFloatingFileExplorer()->GetState() == EditorUi::FileExplorerState::Write) 
	{
		EditorUi::Editor::GetFloatingFileExplorer()->DrawAndSave(content);
	}
}