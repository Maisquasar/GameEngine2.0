#include "Include/EditorUi/Inspector.h"
#include "Include/Utils/Input.h"
#include "Include/App.h"

std::vector<Core::Node*> EditorUi::Inspector::Selected;

EditorUi::Inspector::Inspector()
{
}

EditorUi::Inspector::~Inspector()
{
}

Core::Components::Component* ComponentsPopup()
{
	if (ImGui::BeginPopup("Components", ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		for (auto component : App::Components.Components)
		{
			if (ImGui::Button(component->ComponentName.c_str())) {
				ImGui::EndPopup();
				return component;
			}
		}
		ImGui::EndPopup();
	}
	return nullptr;
}

void EditorUi::Inspector::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Inspector", &_open))
	{
		if (Selected.size() == 1)
		{
			ImGui::Checkbox("##active", Selected[0]->GetActivePtr());
			ImGui::SameLine();
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
			ImGui::NewLine();
			ImGui::Separator();

			// Other Components
			for (auto component : Selected[0]->Components)
			{
				if (ImGui::CollapsingHeader(component->ComponentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					component->ShowInInspector();
				}
				ImGui::NewLine();
				ImGui::Separator();
			}
			ImGui::NewLine();
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 100);
			if (ImGui::Button("New Component", ImVec2(200, 0)))
			{
				ImGui::OpenPopup("Components");
			}
			if (auto comp = ComponentsPopup())
			{
				Selected[0]->AddComponent(comp);
			}
		}
	}
	ImGui::End();
}

void EditorUi::Inspector::AddSelected(Core::Node* node)
{
	Selected.push_back(node);
}
