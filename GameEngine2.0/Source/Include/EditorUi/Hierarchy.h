#pragma once
#include <ImGui/imgui.h>
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
		Core::Node* _rightClicked = nullptr;
		bool _openRightClick;

	};
}