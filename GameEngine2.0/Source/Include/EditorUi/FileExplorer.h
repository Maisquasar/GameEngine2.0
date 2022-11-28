#pragma once
#include <string>
#include <vector>
#include <filesystem>

#include "ImGui/imgui.h"
#include "EditorWindow.h"
#include "Include/Resources/ResourceManager.h"

namespace EditorUi {

	enum class FileType
	{
		Folder,
		None,
		Txt,
		Img
	};

	enum class FileExplorerState
	{
		Read,
		Write
	};

	class File
	{
	public:
		File();
		File(std::string filename);
		~File();
		std::string Name;
		std::string Directory;
		FileType Type = FileType::None;
		std::vector<std::shared_ptr<File>> Children;
		bool Selected = false;

		Resources::Texture* Icon = nullptr;
		Resources::IResource* ResourceLink = nullptr;

		void FoundChildren();
		std::shared_ptr<File> GetParent();

	private:

	};

	class FloatingFileExplorer : public EditorWindow
	{
	public:
		FloatingFileExplorer();
		~FloatingFileExplorer();

		std::shared_ptr<File> DrawAndRead();
		void DrawAndSave(const char* data);
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

	class FileExplorer : public FloatingFileExplorer
	{
	public:
		FileExplorer();
		~FileExplorer();

		void Draw() override;
	};
}