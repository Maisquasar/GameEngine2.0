#pragma once
#include <ImGui/imgui.h>
#include "Include/EditorUi/Hierarchy.h"
#include "Include/EditorUi/MainBar.h"
#include "Include/EditorUi/FileExplorer.h"
#include "Include/EditorUi/Console.h"

namespace EditorUi{
	class Editor
	{
	public:
		Editor();
		~Editor();

		void Draw();

		static FileExplorer* GetFileExplorer() { return &_fileExplorer; }
	private:
		MainBar _mainBar;
		Hierarchy _hierarchy;
		static FileExplorer _fileExplorer;

	};
}
