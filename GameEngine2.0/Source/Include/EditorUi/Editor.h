#pragma once
#include <ImGui/imgui.h>
#include "Include/EditorUi/Hierarchy.h"
#include "Include/EditorUi/MainBar.h"
#include "Include/EditorUi/FileExplorer.h"
#include "Include/EditorUi/Console.h"

namespace EditorUi {
	void ApplyStyle();
	class Editor
	{
	public:
		Editor();
		~Editor();

		void Initialize();

		void Draw();
		void MainDocking();

		static FileExplorer* GetFileExplorer() { return &_fileExplorer; }
		static Hierarchy* GetHierarchy() { return &_hierarchy; }
	private:
		MainBar _mainBar;
		static Hierarchy _hierarchy;
		static FileExplorer _fileExplorer;

	};
}
