#include "Include/EditorUi/Hierarchy.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/Resources/Model.h"
#include "Include/App.h"
#include "Include/Utils/Input.h"
#include "Include/Core/Node.h"
#include "Include/EditorUi/SceneWindow.h"


EditorUi::Hierarchy::Hierarchy() {}

EditorUi::Hierarchy::~Hierarchy() {}

void EditorUi::Hierarchy::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Hierarchy", &_open))
	{
		ImGui::BeginChild("Child");
		Application.GetScene()->GetSceneNode()->ShowInHierarchy(0);
		// RightClick
		if (_openRightClick)
		{
			_openRightClick = false;
			ImGui::OpenPopup("RightClick");
		}
		RightClickWindow();
		// Copy
		if (ImGui::IsWindowFocused() || EditorUi::Editor::GetSceneWindow()->IsFocused) {
			if (Inspector::NodesSelected.size() == 1 && (Utils::Input::IsKeyDown(ImGuiKey_LeftCtrl) && Utils::Input::IsKeyPressed(ImGuiKey_C)))
			{
				// Save Scene To Temporary Scene.
				Application.GetScene()->SaveNode("Assets/Default/Scenes/Clipboard.scene", Inspector::NodesSelected[0]);
				PrintLog("Copy");
			}
			// Cut
			else if (Inspector::NodesSelected.size() == 1 && (Utils::Input::IsKeyDown(ImGuiKey_LeftCtrl) && Utils::Input::IsKeyPressed(ImGuiKey_X)))
			{
				Application.GetScene()->SaveNode("Assets/Default/Scenes/Clipboard.scene", Inspector::NodesSelected[0]);
				Inspector::NodesSelected[0]->RemoveFromParent();
				Inspector::NodesSelected.erase(Inspector::NodesSelected.begin());
				Application.GetScene()->GetGizmo()->NodeTransform = nullptr;
				PrintLog("Cut");
			}
			// Paste
			else if (Inspector::NodesSelected.size() == 1 && (Utils::Input::IsKeyDown(ImGuiKey_LeftCtrl) && Utils::Input::IsKeyPressed(ImGuiKey_V)))
			{
				Inspector::NodesSelected[0]->AddChildren(Application.GetScene()->LoadNode("Assets/Default/Scenes/Clipboard.scene"));
				PrintLog("Paste");
			}
			// Delete
			else if (Inspector::NodesSelected.size() == 1 && (Utils::Input::IsKeyPressed(ImGuiKey_Delete)))
			{
				Inspector::NodesSelected[0]->RemoveFromParent();
				Inspector::NodesSelected.erase(Inspector::NodesSelected.begin());
				Application.GetScene()->GetGizmo()->NodeTransform = nullptr;
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
			for (auto node : _rightClicked) {
				node->AddChildren(new Core::Node());
			}
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("New Parent Node"))
		{
			for (auto node : _rightClicked) {
				if (!node->Parent)
					break;
				auto parent = new Core::Node();
				node->Parent->AddChildren(parent);
				node->SetParent(parent);
			}
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Load Model"))
		{
			ImGui::OpenPopup("ModelPopup");
		}
		if (auto model = Application.GetResourceManager()->ResourcesPopup<Resources::Model>("ModelPopup"))
		{
			auto m = model->CloneNode();
			//_rightClicked[0]->AddChildren(m);
			_rightClicked[0]->AddChildren(m);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Clear Parent"))
		{
			for (auto node : _rightClicked) {
				if (node->Parent)
					node->SetParent(Application.GetScene()->GetSceneNode());
			}
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Delete"))
		{
			_rightClicked[0]->RemoveFromParent();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void EditorUi::Hierarchy::SetRightClicked(Core::Node* node)
{
	_openRightClick = true;
	if (_rightClicked.size() == 0)
		_rightClicked.push_back(node);
	else
		_rightClicked[0] = node;
}
