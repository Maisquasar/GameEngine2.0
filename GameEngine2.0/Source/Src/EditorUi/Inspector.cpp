#include "Include/EditorUi/Inspector.h"
#include "Include/Utils/Input.h"
#include "Include/App.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/EditorUi/FileExplorer.h"
#include "Include/EditorUi/File.h"
#include "Include/Core/Node.h"

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
		for (auto component : Application.Components.Components)
		{
			if (auto icon = component->GetUIIcon()) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.f);
				ImGui::Image((ImTextureID)static_cast<uintptr_t>(icon->GetData()), ImVec2(16, 16));
				ImGui::SameLine();
			}
			if (ImGui::Selectable(component->ComponentName.c_str(), false, ImGuiSelectableFlags_SpanAvailWidth)) {
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
			if (ImGui::InputText("Name", name, 64, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				NodesSelected[0]->Name = name;
			}
			// Transform
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
			{
				NodesSelected[0]->Transform.ShowInInspector();
			}
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
				bool open = ImGui::CollapsingHeader(component->ComponentName.c_str(), &destroy, ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
				if (auto icon = component->GetUIIcon()) {
					ImGui::SameLine();
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1.f);
					float cx = ImGui::GetCursorPosX();
					float x = ImGui::GetWindowSize().x * 7/8;
					ImGui::SetCursorPosX(x);
					ImGui::Image((ImTextureID)static_cast<uintptr_t>(icon->GetData()), ImVec2(16, 16));
				}
				if (open) {
					ImGui::BeginDisabled(!component->IsEnable());
					ImGui::TreePush(component->ComponentName.c_str());
					component->ShowInInspector();
					ImGui::TreePop();
					ImGui::EndDisabled();
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
	Application.GetScene()->GetGizmo()->NodeTransform = &node->Transform;
	node->SetSelected(true);
	FileSelected = nullptr;
}

void EditorUi::Inspector::ClearSelected()
{
	for (auto node : NodesSelected) node->SetSelected(false);
	NodesSelected.clear();
	FileSelected = nullptr;
	Application.GetScene()->GetGizmo()->NodeTransform = nullptr;
}

void EditorUi::Inspector::SetFileSelected(EditorUi::File* file)
{
	FileSelected = file;
	for (auto node : NodesSelected) node->SetSelected(false);
	NodesSelected.clear();
	Application.GetScene()->GetGizmo()->NodeTransform = nullptr;
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
