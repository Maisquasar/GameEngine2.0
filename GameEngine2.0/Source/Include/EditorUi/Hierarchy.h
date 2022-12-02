#pragma once
#include <ImGui/imgui.h>
#include <vector>
#include "EditorWindow.h"

namespace Core {
	class Node;
}

namespace EditorUi {
	class Hierarchy : public EditorWindow
	{
	public:
		Hierarchy();
		~Hierarchy();

		void Draw() override;
		void RightClickWindow();
		void SetRightClicked(Core::Node* node);

	private:
		std::vector<Core::Node*> _rightClicked;
		bool _openRightClick = false;

	};
}