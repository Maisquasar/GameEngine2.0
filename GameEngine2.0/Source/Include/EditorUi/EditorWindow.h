#pragma once
namespace EditorUi {
	class EditorWindow
	{
	public:
		EditorWindow() {}
		~EditorWindow() {}
		
		virtual void Draw() {}

		void SetOpen(bool value) { _open = value; }
	protected:
		bool _open = true;

	};
}