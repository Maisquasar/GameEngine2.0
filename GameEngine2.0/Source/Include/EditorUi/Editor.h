#pragma once
#include <ImGui/imgui.h>
namespace ImGui
{
	class NodeWindow;
}

namespace EditorUi {
	class MainBar;
	class FileExplorer;
	class FloatingFileExplorer;
	class Hierarchy;
	class Inspector;
	class ResourcesWindow;
	class PerformanceWindow;
	class Console;

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
		static MainBar _mainBar;
		static Hierarchy _hierarchy;
		static FileExplorer _fileExplorer;
		static FloatingFileExplorer _floatingFileExplorer;
		static Inspector _inspector;
		static ResourcesWindow _resourcesWindow;
		static PerformanceWindow _performanceWindow;
		static ImGui::NodeWindow _nodeWindow;
	};
}
