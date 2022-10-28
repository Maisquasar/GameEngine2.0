#include "Include/EditorUi/Inspector.h"
#include "Include/Utils/Input.h"
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
			// Name Input.
			char name[65];
			strcpy_s(name, 64,Selected[0]->Name.c_str());
			if (ImGui::InputText("Name", name, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (Utils::Input::IsKeyPressed(ImGuiKey_Enter) || Utils::Input::IsKeyPressed(ImGuiKey_KeypadEnter))
					Selected[0]->Name = name;
			}
			// Transform
			Selected[0]->Transform.ShowInInspector();

			// Other Components
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
