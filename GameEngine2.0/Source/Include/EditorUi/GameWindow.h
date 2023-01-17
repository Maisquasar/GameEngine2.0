#pragma once
#include "EditorWindow.h"
#include <ImGui/imgui_internal.h>


namespace EditorUi
{
	class GameWindow : public EditorWindow
	{
	public:
		GameWindow();
		~GameWindow();

		void Draw() override;

		ImGuiWindow* Window;
	private:

	};
}