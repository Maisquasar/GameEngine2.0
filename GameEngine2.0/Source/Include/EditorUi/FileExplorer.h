#pragma once
#include <string>
#include <vector>

#include "ImGui/imgui.h"
#include "EditorWindow.h"

namespace EditorUi {

	enum class FileType
	{
		Folder,
		None,
		Txt,
	};

	class File
	{
	public:
		File();
		~File();
		const char* Name;
		const char* Directory;

	private:

	};

	class FileExplorer : public EditorWindow
	{
	public:
		FileExplorer();
		~FileExplorer();

		void Draw() override;
	private:
		std::string _path;
	};
}