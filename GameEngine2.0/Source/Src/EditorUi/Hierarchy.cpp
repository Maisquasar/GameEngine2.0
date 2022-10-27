#include "Include/EditorUi/Hierarchy.h"
#include "Include/App.h"

EditorUi::Hierarchy::Hierarchy(){}

EditorUi::Hierarchy::~Hierarchy(){}

void EditorUi::Hierarchy::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Hierarchy", &_open))
	{
		ImGui::BeginChild("Child");
		App::SceneNode->ShowInHierarchy(0);
		ImGui::EndChild();
	}
	ImGui::End();
}