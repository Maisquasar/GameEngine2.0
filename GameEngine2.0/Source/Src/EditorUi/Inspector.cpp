#include "Include/EditorUi/Inspector.h"
#include "Include/Utils/Input.h"
#include "Include/App.h"
#include "Include/Core/Components/MeshComponent.h"

std::vector<Core::Node*> EditorUi::Inspector::NodesSelected;
EditorUi::File* EditorUi::Inspector::FileSelected;

EditorUi::Inspector::Inspector()
{
}

EditorUi::Inspector::~Inspector()
{
}

Core::Components::Component* ComponentsPopup()
{
	Core::Components::Component* out = nullptr;
	if (ImGui::BeginPopup("Components", ImGuiWindowFlags_AlwaysVerticalScrollbar))
	{
		for (auto component : App::Components.Components)
		{
			if (ImGui::Button(component->ComponentName.c_str())) {
				out = component->Clone();
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}
	return out;
}

void EditorUi::Inspector::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Inspector", &_open))
	{
		if (NodesSelected.size() == 1 && NodesSelected[0]->Parent)
		{
			ImGui::Checkbox("##active", NodesSelected[0]->GetActivePtr());
			ImGui::SameLine();
			// Name Input.
			char name[65];
			strcpy_s(name, 64, NodesSelected[0]->Name.c_str());
			if (ImGui::InputText("Name", name, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				if (Utils::Input::IsKeyPressed(ImGuiKey_Enter) || Utils::Input::IsKeyPressed(ImGuiKey_KeypadEnter))
					NodesSelected[0]->Name = name;
			}
			// Transform
			NodesSelected[0]->Transform.ShowInInspector();
			ImGui::NewLine();
			ImGui::Separator();

			// Other Components
			int index = 0;
			for (auto component : NodesSelected[0]->Components)
			{
				bool destroy = true;
				ImGui::PushID(index++);
				ImGui::Checkbox("##", component->GetEnable());
				ImGui::SameLine();
				if (ImGui::CollapsingHeader(component->ComponentName.c_str(), &destroy, ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen)) {
					component->ShowInInspector();
				}
				ImGui::NewLine();
				ImGui::Separator();
				ImGui::PopID();
				if (!destroy)
					component->RemoveFromParent();
			}
			// New Component Button
			ImGui::NewLine();
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - 100);
			if (ImGui::Button("New Component", ImVec2(200, 0)))
			{
				ImGui::OpenPopup("Components");
			}
			if (auto comp = ComponentsPopup())
			{
				NodesSelected[0]->AddComponent(comp);
			}
		}
		else if (FileSelected)
		{
			FileSelected->ShowInInspector();
		}
	}
	ImGui::End();
}

void EditorUi::Inspector::AddNodeSelected(Core::Node* node)
{
	NodesSelected.push_back(node);
	node->SetSelected(true);
	FileSelected = nullptr;
}

void EditorUi::Inspector::SetFileSelected(EditorUi::File* file)
{
	FileSelected = file;
	for (auto node : NodesSelected) node->SetSelected(false);
	NodesSelected.clear();
}

bool EditorUi::Inspector::IsSelected(Core::Node* node)
{
	for (auto SelectedNode : NodesSelected)
	{
		if (SelectedNode == node)
			return true;
	}
	return false;
}
