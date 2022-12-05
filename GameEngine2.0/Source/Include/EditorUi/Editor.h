#pragma once
#include <ImGui/imgui.h>
#include "Include/EditorUi/Hierarchy.h"
#include "Include/EditorUi/MainBar.h"
#include "Include/EditorUi/FileExplorer.h"
#include "Include/EditorUi/Console.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/EditorUi/ResourcesWindow.h"
#include "Include/EditorUi/PerformanceWindow.h"

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
		static FloatingFileExplorer* GetFloatingFileExplorer() { return &_floatingFileExplorer; }
		static Hierarchy* GetHierarchy() { return &_hierarchy; }
		static Inspector* GetInspector() { return &_inspector; }
		static ResourcesWindow* GetResourcesWindow() { return &_resourcesWindow; }
		static PerformanceWindow* GetPerformanceWindow() { return &_performanceWindow; }
		static Console* GetConsole() { return &Debug::Log::Console; }
	private:
		MainBar _mainBar;
		static Hierarchy _hierarchy;
		static FileExplorer _fileExplorer;
		static FloatingFileExplorer _floatingFileExplorer;
		static Inspector _inspector;
		static ResourcesWindow _resourcesWindow;
		static PerformanceWindow _performanceWindow;
	};
}
