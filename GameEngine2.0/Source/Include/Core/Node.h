#pragma once
#include "Transform.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include "Components/Component.h"

namespace Core {
	class Node
	{
	public:
		Node();
		~Node();

		Node* Parent = nullptr;
		std::vector<std::shared_ptr<Node>> Childrens;
		std::vector<Components::Component*> Components;
		Core::Transform Transform;

		std::string Name = "Node";

		void AddChildren(Node* node);
		void AddComponent(Core::Components::Component* comp);

		void RemoveChildren(Node* node);
		void RemoveFromParent();
		void RemoveComponent(Core::Components::Component* comp);
		void RemoveAllChildrens();

		bool IsAParent(Node* node);

		void UpdateSelfAndChilds();

		void ShowInHierarchy(int index);

		template<typename T> T* GetComponent();
		bool* GetActivePtr() { return &_active; }

		void SetSelected(bool value) { _selected = value; }

		void Save(std::string space, std::string& content);
		void Load(const char* data, uint32_t& pos);
	private:
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
}