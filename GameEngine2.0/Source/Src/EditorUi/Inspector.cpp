#include "Include/EditorUi/Inspector.h"
std::vector<Core::Node*> EditorUi::Inspector::Selected;

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
		if (Selected.size() == 1)
		{
			Selected[0]->Transform.ShowInInspector();

			for (auto component : Selected[0]->Components)
			{
				if (ImGui::CollapsingHeader(component->ComponentName.c_str())) {

				}
			}
		}
	}
	ImGui::End();
}

void EditorUi::Inspector::AddSelected(Core::Node* node)
{
	Selected.push_back(node);
}
