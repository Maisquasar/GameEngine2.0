#pragma once
#include <ImGui/imgui.h>

namespace EditorUi {
	class EditorWindow
	{
	public:
		EditorWindow() {}
		~EditorWindow() {}
		
		virtual void Draw() {}

		virtual void SetOpen(bool value) { _open = value; }
	protected:
		bool _open = true;

	};
}