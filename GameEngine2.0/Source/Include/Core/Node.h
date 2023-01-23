#pragma once
#include "Transform.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "Components/Component.h"
namespace Resources
{
	class Animation;
}

namespace Core {
	class Node
	{
	public:
		Node();
		virtual ~Node();

		Node* Parent = nullptr;
		std::vector<Node*> Childrens;
		std::vector<Components::Component*> Components;
		Core::Transform Transform;
		virtual void Test() {}

		std::string Name = "Node";

		void AddChildren(Node* node);
		void AddComponent(Core::Components::Component* comp);
		void SetParent(Node* node);

		// Clone this Node and his children.
		virtual Node* CloneNode();

		void RemoveChildren(Node* node);
		void RemoveFromParent();
		void RemoveComponent(Core::Components::Component* comp);
		void RemoveAllChildrens();

		bool IsAParent(Node* node);
		bool IsSelected() { return _selected; }

		void EndPause();

		void UpdateSelfAndChilds();
		void DrawSelfAndChild(bool editorCamera = false);
		void DrawPicking(int id);
		virtual void DrawDebug() {}
		virtual void UpdateBone(Resources::Animation*, float) {}

		void ShowInHierarchy(int index);

		template<typename T> T* GetComponent();
		template<typename T> std::vector<T*> GetComponents();
		template<typename T> std::vector<T*> GetChildrensComponents();
		std::vector<Core::Node*> GetAllChildrens();
		bool* GetActivePtr() { return &_active; }

		void SetSelected(bool value) { _selected = value; }

		virtual void Save(std::string space, std::string& content);
		virtual void Load(const char* data, uint32_t& pos);
	protected:
		bool _open = false;
		bool _active = true; // Boolean for inspector active
		bool _selected = false;

	};
	template<typename T>
	inline T* Node::GetComponent()
	{
		for (auto comp : Components)
		{
			if (auto value = dynamic_cast<T*>(comp))
			{
				return value;
			}
		}
		return nullptr;
	}

	template<typename T>
	inline std::vector<T*> Node::GetComponents()
	{
		std::vector<T*> OutComponents;
		for (auto component : Components)
		{
			if (auto type = dynamic_cast<T*>(component))
			{
				OutComponents.push_back(type);
			}
		}
		return OutComponents;
	}

	template<typename T>
	inline std::vector<T*> Node::GetChildrensComponents()
	{
		std::vector<T*> components;
		auto nodeComp = GetComponents<T>();
		components.insert(components.end(), nodeComp.begin(), nodeComp.end());

		// Recursively search the children of the node
		for (auto child : Childrens)
		{
			std::vector<T*> childComponents = child->GetChildrensComponents<T>();
			components.insert(components.end(), childComponents.begin(), childComponents.end());
		}

		return components;
	}
}