#include "Include/EditorUi/Hierarchy.h"
#include "Include/App.h"
#include "Include/Utils/Input.h"
#include "Include/Core/Node.h"

EditorUi::Hierarchy::Hierarchy() {}

EditorUi::Hierarchy::~Hierarchy() {}

void EditorUi::Hierarchy::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Hierarchy", &_open))
	{
		ImGui::BeginChild("Child");
		App::SceneNode->ShowInHierarchy(0);
		// RightClick
		if (_openRightClick)
		{
			_openRightClick = false;
			ImGui::OpenPopup("RightClick");
		}
		RightClickWindow();
		// Copy
		if (Inspector::NodesSelected.size() == 1 && (Utils::Input::IsKeyDown(ImGuiKey_LeftCtrl) && Utils::Input::IsKeyPressed(ImGuiKey_C)))
		{
			// Save Scene To Temporary Scene.
			App::SaveNode("Assets/Default/Scenes/Clipboard.scene", Inspector::NodesSelected[0]);
			PrintLog("Copy");
		}
		// Paste
		else if (Inspector::NodesSelected.size() == 1 && (Utils::Input::IsKeyDown(ImGuiKey_LeftCtrl) && Utils::Input::IsKeyPressed(ImGuiKey_V)))
		{
			Inspector::NodesSelected[0]->AddChildren(App::LoadNode("Assets/Default/Scenes/Clipboard.scene"));
			PrintLog("Paste");
		}
		// Delete
		else if (Inspector::NodesSelected.size() >= 1 && (Utils::Input::IsKeyPressed(ImGuiKey_Delete)))
		{
			for (auto Node : Inspector::NodesSelected)
			{
				Node->RemoveFromParent();
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void EditorUi::Hierarchy::RightClickWindow()
{
	if (ImGui::BeginPopup("RightClick", ImGuiWindowFlags_NoDecoration))
	{
		if (ImGui::Button("New Node"))
		{
			_rightClicked->AddChildren(new Core::Node());
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Delete"))
		{
			_rightClicked->RemoveFromParent();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void EditorUi::Hierarchy::SetRightClicked(Core::Node* node)
{
	_openRightClick = true;
	_rightClicked = node;
}
