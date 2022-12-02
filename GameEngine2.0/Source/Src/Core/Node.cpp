#include "Include/Core/Node.h"
#include "Include/Utils/Loader.h"
#include "Include/App.h"

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

void Core::Node::RemoveChildren(Node* node)
{
	int index = 0;
	for (auto child : Childrens)
	{
		if (node == child.get())
		{
			Childrens.erase(Childrens.begin() + index);
			break;
		}
		index++;
	}
}

void Core::Node::RemoveFromParent()
{
	if (!Parent)
		return;
	Parent->RemoveChildren(this);
	int index = 0;
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

void Core::Node::RemoveAllChildrens()
{
	for (auto child : Childrens)
	{
		child->RemoveAllChildrens();
	}
	Childrens.clear();
}

bool Core::Node::IsAParent(Node* node)
{
	if (!Parent) {
		if (Parent == node)
			return true;
		return Parent->IsAParent(node);
	}
	return false;
}

void Core::Node::UpdateSelfAndChilds()
{
	this->Transform.Update();
	for (auto component : this->Components)
	{
		if (component->IsEnable())
		{
			switch (App::GetGameState())
			{
			case GameState::Play:
				component->GameUpdate();
				break;
			case GameState::Editor:
				component->EditorUpdate();
				break;
			default:
				break;
			}
			component->Update();
		}
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
			for (auto i : EditorUi::Inspector::NodesSelected)
				i->_selected = false;
			EditorUi::Inspector::NodesSelected.clear();
		}
		EditorUi::Inspector::AddNodeSelected(this);
	}
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		EditorUi::Editor::GetHierarchy()->SetRightClicked(this);
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

void Core::Node::Save(std::string space, std::string& content)
{
	// Node Save.
	content += space + "#BeginNode\n";
	content += space + Utils::Loader::StringFormat("Name : %s\n", Name.c_str());
	content += space + Utils::Loader::StringFormat("IsActive : %d\n", _active);

	// Components Save.
	Transform.Save(space, content);
	for (auto component : Components)
	{
		content += space + Utils::Loader::StringFormat("#BeginComponent %s\n", component->ComponentName.c_str());
		component->Save(space, content);
		content += space + Utils::Loader::StringFormat("#EndComponent %s\n", component->ComponentName.c_str());
	}

	// Children Save.
	for (auto children : Childrens)
	{
		children->Save(space + '\t', content);
	}
	content += space + "#EndNode\n";
}

void Core::Node::Load(const char* data, uint32_t& pos)
{
	std::string currentLine;
	while (currentLine.substr(0, 8) != "#EndNode")
	{
		currentLine = Utils::Loader::GetLine(data, pos);
		if (currentLine.substr(0, 4) == "Name")
		{
			this->Name = Utils::Loader::GetString(currentLine);
		}
		else if (currentLine.substr(0, 8) == "IsActive")
		{
			this->_active = Utils::Loader::GetInt(currentLine);
		}
		else if (currentLine.substr(0, 8) == "Position")
		{
			this->Transform.SetLocalPosition(Utils::Loader::GetVector3(currentLine));
		}
		else if (currentLine.substr(0, 8) == "Rotation")
		{
			this->Transform.SetLocalRotation(Utils::Loader::GetVec4(currentLine));
		}
		else if (currentLine.substr(0, 5) == "Scale")
		{
			this->Transform.SetLocalScale(Utils::Loader::GetVector3(currentLine));
		}
		else if (currentLine.substr(0, 10) == "#BeginNode")
		{
			this->AddChildren(new Node());
			this->Childrens.back()->Load(data, pos);
		}
		else if (currentLine.substr(0, 15) == "#BeginComponent")
		{
			auto Type = Utils::Loader::GetString(currentLine);
			for (auto component : App::Components.Components)
			{
				if (Type == component->ComponentName)
				{
					auto newComponent = component->Clone();
					newComponent->Load(data, pos);
					this->AddComponent(newComponent);
					break;
				}
			}
			continue;
		}
		else if (currentLine.substr(0, 8) == "#EndNode")
		{
			break;
		}
		pos++;
	}
}
