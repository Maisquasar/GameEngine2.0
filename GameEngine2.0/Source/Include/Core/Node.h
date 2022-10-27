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

		std::shared_ptr<Node> Parent;
		std::vector<std::shared_ptr<Node>> Childrens;
		std::vector<std::shared_ptr<Component::Component>> Components;
		Core::Transform Transform;

		std::string Name = "Node";

		void AddChildren(Node* node);

		void ShowInHierarchy(int index);

	private:
		bool _open = false;
		bool _selected = false;

	};
}