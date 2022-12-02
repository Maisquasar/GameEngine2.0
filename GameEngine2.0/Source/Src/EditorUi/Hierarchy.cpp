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
		if (Inspector::NodesSelected.size() == 1 && (Utils::Input::IsKeyDown(ImGuiKey_LeftCtrl) && Utils::Input::IsKeyPressed(ImGuiKey_C)))
		{
			// Save Scene To Temporary Scene.
			App::SaveNode("Assets/Default/Scenes/Clipboard.scene", Inspector::NodesSelected[0]);
			PrintLog("Copy");
		}
		else if (Inspector::NodesSelected.size() == 1 && (Utils::Input::IsKeyDown(ImGuiKey_LeftCtrl) && Utils::Input::IsKeyPressed(ImGuiKey_V)))
		{
			Inspector::NodesSelected[0]->AddChildren(App::LoadNode("Assets/Default/Scenes/Clipboard.scene"));
			PrintLog("Paste");
		}
		else if (Inspector::NodesSelected.size() >= 1 && (Utils::Input::IsKeyPressed(ImGuiKey_Delete)))
		{
			for (auto Node : Inspector::NodesSelected)
			{
				Node->RemoveFromParent();
			}
			Inspector::SetFileSelected(nullptr);
		}
		ImGui::EndChild();
	}
	ImGui::End();
}