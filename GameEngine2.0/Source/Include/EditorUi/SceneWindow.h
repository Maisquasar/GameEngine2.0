#pragma once
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#include "EditorWindow.h"
#include "Include/Math/Math.h"

namespace EditorUi {
	class SceneWindow : public EditorWindow
	{
	public:
		SceneWindow();
		~SceneWindow();

		void Draw() override;

		Math::Vec2 GetSize();
		Math::Vec2 GetPos();

		Math::Vec2 GetMousePosition();

		bool UpdateCameraEditor = false;
		bool IsHovered = false;
		bool IsFocused = false;

		ImGuiWindow* Window = nullptr;

	private:
		Math::Vec2 _size;
		Math::Vec2 _pos;
	};
}