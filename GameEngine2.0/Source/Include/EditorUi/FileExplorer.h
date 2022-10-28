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

		void Draw() override;
		void SetOpen(bool value) override;
		void Refresh();
	protected:
		std::string _windowName;
		bool _limited;
		std::string _path = "Assets";
		std::shared_ptr<File> _current = nullptr;
		std::shared_ptr<File> _main;

		std::shared_ptr<File> _clicked = nullptr;
		void RightClickWindow();
	};

	class FileExplorer : public FloatingFileExplorer
	{
	public:
		FileExplorer();
		~FileExplorer();
	};
}