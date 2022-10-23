#include "Include/EditorUi/Hierarchy.h"

EditorUi::Hierarchy::Hierarchy(){}

EditorUi::Hierarchy::~Hierarchy(){}

void EditorUi::Hierarchy::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Hierarchy", &_open))
	{

	}
	ImGui::End();
}