#pragma once
#include <string>
#include <memory>
#include <vector>
#include <filesystem>

namespace Resources
{
	class Texture;
	class IResource;
}

namespace EditorUi {
	enum class FileType
	{
		Folder,
		None,
		Txt,
		Img,
		Mat,
		Mtl,
		Obj,
		Scn,
		Shdr,
		Phm,
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
		void FindAllChilden();
		std::shared_ptr<File> GetParent();

		void DrawInFileExplorer(File*& clicked);

		void ShowInInspector();

	private:

	};
}