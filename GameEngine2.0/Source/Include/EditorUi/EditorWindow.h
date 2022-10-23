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
		virtual bool* IsOpen() { return &_open; }
	protected:
		bool _open = true;

	};
}