#pragma once
#include <memory>
#include <string>

#include "File.h"
#include "ImGui/imgui.h"
#include "EditorWindow.h"
#include "Include/Debug/Log.h"


namespace EditorUi {
	enum class FileExplorerState
	{
		Read,
		Write
	};

	class FloatingFileExplorer : public EditorWindow
	{
	public:
		FloatingFileExplorer();
		~FloatingFileExplorer();

		std::shared_ptr<File> DrawAndRead();
		void DrawAndSave(std::string data);
		void Draw() override;
		void Refresh();

		void SetOpen(bool value) override;
		void SetTargetExtension(std::string ext);
		void SetState(FileExplorerState state);

		FileExplorerState GetState();
	protected:
		std::string _windowName;
		std::string _path = "Assets";
		std::string _targetExtension = "";
		std::shared_ptr<File> _current = nullptr;
		std::shared_ptr<File> _main;
		std::shared_ptr<File> _rightClicked = nullptr;

		FileExplorerState _state;
		void RightClickWindow();
	};
}