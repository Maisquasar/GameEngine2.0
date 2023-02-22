#include "stdafx.h"
#include "Include/EditorUi/Editor.h"
#include "Include/EditorUi/Hierarchy.h"
#include "Include/EditorUi/MainBar.h"
#include "Include/EditorUi/FileExplorer.h"
#include "Include/EditorUi/Console.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/EditorUi/ResourcesWindow.h"
#include "Include/EditorUi/PerformanceWindow.h"
#include "ImGui/Nodes/NodeWindow.h"
#include "Include/EditorUi/Console.h"
#include "Include/EditorUi/AnimationWindow.h"
#include "Include/EditorUi/SceneWindow.h"
#include "Include/EditorUi/GameWindow.h"

EditorUi::MainBar EditorUi::Editor::_mainBar;
EditorUi::FileExplorer EditorUi::Editor::_fileExplorer;
EditorUi::FloatingFileExplorer EditorUi::Editor::_floatingFileExplorer;
EditorUi::Hierarchy EditorUi::Editor::_hierarchy;
EditorUi::Inspector EditorUi::Editor::_inspector;
EditorUi::ResourcesWindow EditorUi::Editor::_resourcesWindow;
EditorUi::PerformanceWindow EditorUi::Editor::_performanceWindow;
ImGui::NodeWindow EditorUi::Editor::_nodeWindow;
TextEditorWindow EditorUi::Editor::_textEditor;
EditorUi::AnimationWindow EditorUi::Editor::_animationWindow;
EditorUi::SceneWindow EditorUi::Editor::_sceneWindow;
EditorUi::GameWindow EditorUi::Editor::_gameWindow;

EditorUi::Editor::Editor() {}

EditorUi::Editor::~Editor() {}

void EditorUi::Editor::Initialize()
{
	_fileExplorer.SetOpen(true);
	ApplyStyle();
}

void EditorUi::Editor::Draw()
{
	//ImGui::ShowDemoWindow();
	MainDocking();
	_mainBar.Draw();
	_hierarchy.Draw();
	Debug::Log::Console.Draw();
	_inspector.Draw();
	_resourcesWindow.Draw();
	_fileExplorer.Draw();
	_performanceWindow.Draw();
	_textEditor.Draw();
	_animationWindow.Draw();
	_gameWindow.Draw();
	_sceneWindow.Draw();
}

void EditorUi::Editor::MainDocking()
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	ImGui::GetWindowDockID();

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", (bool*)true, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	ImGui::End();
}

EditorUi::Console* EditorUi::Editor::GetConsole()
{
	return &Debug::Log::Console;
}

void EditorUi::ApplyStyle()
{
	auto style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text]					= ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]			= ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg]				= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_ChildBg]				= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_PopupBg]				= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_Border]					= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow]			= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg]				= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_FrameBgHovered]			= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_FrameBgActive]			= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	colors[ImGuiCol_TitleBg]				= ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	colors[ImGuiCol_TitleBgActive]			= ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]		= ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg]				= ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg]			= ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]			= ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]	= ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]	= ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark]				= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	colors[ImGuiCol_SliderGrab]				= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]		= ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
	colors[ImGuiCol_Button]					= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_ButtonHovered]			= ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_ButtonActive]			= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	colors[ImGuiCol_Header]					= ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_HeaderHovered]			= ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_HeaderActive]			= ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	colors[ImGuiCol_Separator]				= ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered]		= ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	colors[ImGuiCol_SeparatorActive]		= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_ResizeGrip]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_ResizeGripHovered]		= ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]		= ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab]					= ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	colors[ImGuiCol_TabHovered]				= ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
	colors[ImGuiCol_TabActive]				= ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	colors[ImGuiCol_TabUnfocused]			= ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive]		= ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	colors[ImGuiCol_DockingPreview]			= ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	colors[ImGuiCol_DockingEmptyBg]			= ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines]				= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]		= ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]			= ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]	= ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]			= ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong]		= ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight]		= ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg]				= ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt]			= ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg]			= ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget]			= ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	colors[ImGuiCol_NavHighlight]			= ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight]	= ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg]		= ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style.GrabRounding = style.FrameRounding = 2.3f;
}