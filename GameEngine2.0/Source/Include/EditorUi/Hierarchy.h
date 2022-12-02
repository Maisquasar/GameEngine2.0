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

	private:

	};
}