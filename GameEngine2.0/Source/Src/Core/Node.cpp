#include "Include/Core/Node.h"
#include "Include/Utils/Loader.h"
#include "Include/App.h"
#include "Include/EditorUi/Hierarchy.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/Core/Components/MeshComponent.h"
#include "Include/Resources/Skeleton.h"

Core::Node::Node()
{
	this->Transform.GameObject = this;
}

Core::Node::~Node()
{
	for (int i = (int)Components.size() - 1; i >= 0; i--)
	{
		auto comp = Components[i];
		Components.erase(Components.begin() + i);
		delete comp;
	}
	for (auto child : Childrens)
	{
		delete child;
	}
}

void Core::Node::AddChildren(Node* node)
{
	this->Childrens.push_back(node);
	node->Parent = this;
	node->Transform.Parent = this;
}

void Core::Node::AddComponent(Core::Components::Component* comp)
{
	this->Components.push_back(comp);
	comp->SetGameobject(this);
	comp->Initialize();
}

void Core::Node::SetParent(Node* node)
{
	if (Parent) {
		// Remove this transform from the current parent's children list
		auto it = std::find(Parent->Childrens.begin(), Parent->Childrens.end(), this);
		if (it != Parent->Childrens.end())
		{
			Parent->Childrens.erase(it);
		}
	}

	// Set the new parent and add this transform to the new parent's children list
	Parent = node;
	this->Transform.Parent = Parent;
	if (Parent)
	{
		Parent->AddChildren(this);
	}
}

Core::Node* Core::Node::CloneNode()
{
	auto node = new Node(static_cast<Node const&>(*this));
	node->Childrens.clear();
	node->Components.clear();
	node->Transform.GameObject = node;
	for (auto comp : Components)
	{
		node->AddComponent(comp->Clone());
	}
	for (auto child : Childrens)
	{
		auto newChild = child->CloneNode();
		node->AddChildren(newChild);
	}
	return node;
}

void Core::Node::RemoveChildren(Node* node)
{
	auto it = std::find(Childrens.begin(), Childrens.end(), node);
	if (it != Childrens.end())
	{
		delete node;
		// Remove the child from the list of children
		Childrens.erase(it);
	}
}

void Core::Node::RemoveFromParent()
{
	if (!Parent)
		return;
	Parent->RemoveChildren(this);
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
		delete child;
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
			switch (Application.GetGameState())
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
		if (!child->_active)
			continue;
		child->UpdateSelfAndChilds();
	}
}

void Core::Node::DrawSelfAndChild(bool editorCamera)
{
	for (auto component : this->Components)
	{
		if (component->IsEnable())
		{
			if (!editorCamera)
				component->GameDraw();
			else
				component->EditorDraw();
			component->Draw();
		}
	}
	for (auto child : this->Childrens)
	{
		if (!child->_active)
			continue;
		child->DrawSelfAndChild(editorCamera);
	}
}

#include "Include/Core/Components/SkeletalMeshComponent.h"
void Core::Node::DrawPicking(int id)
{
	if (_active) {
		for (auto comp : Components)
		{
			comp->DrawPicking(id);
		}
	}
	
}


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

std::vector<Core::Node*> Core::Node::GetAllChildrens()
{
	std::vector<Core::Node*> node;
	for (auto child : Childrens)
	{
		node.push_back(child);
		auto cc = child->GetAllChildrens();
		node.insert(node.begin(), cc.begin(), cc.end());
	}
	return node;
}

void Core::Node::Save(std::string space, std::string& content)
{
	// Node Save.
	content += space + "#BeginNode\n";
	content += space + Utils::StringFormat("Name : %s\n", Name.c_str());
	content += space + Utils::StringFormat("IsActive : %d\n", _active);

	// Components Save.
	Transform.Save(space, content);
	for (auto component : Components)
	{
		content += space + Utils::StringFormat("#BeginComponent %s\n", component->ComponentName.c_str());
		component->Save(space, content);
		content += space + Utils::StringFormat("#EndComponent %s\n", component->ComponentName.c_str());
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
			auto name = Utils::Loader::GetString(currentLine);
			this->Name = name;
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
		else if (currentLine.substr(0, 10) == "#BeginBone")
		{
			this->AddChildren(new Bone());
			dynamic_cast<Bone*>(this->Childrens.back())->Load(data, pos);
		}
		else if (currentLine.substr(0, 15) == "#BeginComponent")
		{
			auto Type = Utils::Loader::GetString(currentLine);
			for (auto component : Application.Components.Components)
			{
				if (Type == component->ComponentName)
				{
					auto newComponent = component->Clone();
					newComponent->GameObject = this;
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
