#include "Include/Core/Node.h"

Core::Node::Node()
{
	this->Transform.GameObject = this;
}

Core::Node::~Node() 
{
	for (auto component : Components)
	{
		delete component;
	}
}

void Core::Node::AddChildren(Node* node)
{
	this->Childrens.push_back(std::shared_ptr<Node>(node));
	node->Parent = this;
	node->Transform.Parent = this;
}

void Core::Node::AddComponent(Core::Components::Component* comp)
{
	this->Components.push_back(comp);
	comp->GameObject = this;
}

void Core::Node::RemoveComponent(Core::Components::Component* comp)
{
	int index = 0;
	if (!comp)
		return;
	for (auto component : Components)
	{
#pragma warning(push)
#pragma warning(disable: 6001)
		if (component == comp)
#pragma warning(pop)
		{
			Components.erase(Components.begin() + index);
			delete comp;
		}
		index++;
	}
}

void Core::Node::UpdateSelfAndChilds()
{
	this->Transform.Update();
	for (auto component : this->Components)
	{
		if (component->IsEnable())
			component->Update();
	}
	for (auto child : this->Childrens)
	{
		child->UpdateSelfAndChilds();
	}
}

#include "Include/EditorUi/Inspector.h"
void Core::Node::ShowInHierarchy(int index)
{
	ImGui::BeginDisabled(!_active);
	ImGui::PushID(index);
	if (Childrens.size() > 0) {
		if (!_open) {
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				_open = true;
			}
		}
		else if (_open) {

			if (ImGui::ArrowButton("##down", ImGuiDir_Down))
			{
				_open = false;
			}
		}
		ImGui::SameLine(0, 10);
	}
	else
	{
		ImGui::Button("#", ImVec2(19, 18));
		ImGui::SameLine();
	}
	if (ImGui::Selectable(Name.c_str(), _selected, ImGuiSelectableFlags_SelectOnClick))
	{
		if (!ImGui::GetIO().KeyCtrl)
		{
			for (auto i : EditorUi::Inspector::Selected)
				i->_selected = false;
			EditorUi::Inspector::Selected.clear();
		}
		EditorUi::Inspector::AddSelected(this);
		_selected = true;
	}

	if (_open)
	{
		for (auto&& child : Childrens)
		{
			index++;
			ImGui::TreePush(child->Name.c_str());
			child->ShowInHierarchy(index);
			ImGui::TreePop();
		}
	}
	ImGui::PopID();
	ImGui::EndDisabled();
}
