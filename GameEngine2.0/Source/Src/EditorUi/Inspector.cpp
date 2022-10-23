#include "Include/EditorUi/Inspector.h"

EditorUi::Inspector::Inspector()
{
}

EditorUi::Inspector::~Inspector()
{
}

void EditorUi::Inspector::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Inspector", &_open))
	{

	}
	ImGui::End();
}