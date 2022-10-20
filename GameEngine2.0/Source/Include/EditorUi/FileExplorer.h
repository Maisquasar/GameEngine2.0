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
		FileType Type;
		File* Parent;
		std::vector<File*> Children;

		Resources::Texture* Icon;
		Resources::IResource* ResourceLink;

		void FoundChildren();

	private:

	};

	class FileExplorer : public EditorWindow
	{
	public:
		FileExplorer();
		~FileExplorer();

		void Draw() override;
		void SetOpen(bool value) override;
		void Refresh();
	private:
		std::string _path = "Assets";
		File* _current = nullptr;
		File* _main;
	};
}