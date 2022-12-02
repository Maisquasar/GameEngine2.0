#include "Include/EditorUi/File.h"
#include "Include/Resources/ResourceManager.h"

// -------------- File --------------
EditorUi::File::File() {}

EditorUi::File::File(std::string filename) {
	Directory = filename;
	this->Name = filename.substr(filename.find_last_of("//\\") + 1);
	std::string extension = this->Name.substr(this->Name.find_last_of(".") + 1);
	if (std::filesystem::is_directory(Directory))
	{
		this->Type = EditorUi::FileType::Folder;
		this->Icon = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/folder_icon.png");
	}
	else if (extension == "png")
	{
		this->Type = EditorUi::FileType::Img;
		if (auto tex = Resources::ResourceManager::Get<Resources::Texture>(this->Directory.c_str())) {
			this->Icon = tex;
			this->ResourceLink = tex;
		}
	}
	else if (extension == "mat")
	{
		this->Type = EditorUi::FileType::Mat;
		if (auto mat = Resources::ResourceManager::Get<Resources::Material>(this->Directory.c_str())) {
			this->Icon = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/material_icon.png");
			this->ResourceLink = mat;
		}
	}
	else if (extension == "scene")
	{
		this->Type = EditorUi::FileType::Scn;
	}
	else if (extension == "mtl")
	{
		this->Type = EditorUi::FileType::Mtl;
		this->Icon = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/mtl_icon.png");
	}
	else if (extension == "obj")
	{
		this->Type = EditorUi::FileType::Obj;
		this->Icon = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/obj_icon.png");
	}
	else
	{
		this->Type = EditorUi::FileType::None;
		this->Icon = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/file_icon.png");
	}
	if (!Icon)
		this->Icon = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/file_icon.png");
}


EditorUi::File::~File() {}

void EditorUi::File::FoundChildren()
{
	Children.clear();
	if (Directory.substr(this->Directory.find_last_of("/") + 1) == this->Directory)
		this->Directory += '/';
	for (const auto& entry : std::filesystem::directory_iterator(this->Directory)) {
		std::string dir = entry.path().generic_string().data();
		try
		{
			Children.push_back(std::make_shared<File>(dir));
		}
		catch (const std::exception&)
		{

		}
	}
}

std::shared_ptr<EditorUi::File> EditorUi::File::GetParent()
{
	auto dir = this->Directory.substr(0, this->Directory.find_last_of("/"));
	auto par = std::make_shared<File>(dir);
	par->FoundChildren();
	return par;
}
void EditorUi::File::ShowInInspector()
{
	if (!ResourceLink)
		return;
	try
	{
		ResourceLink->ShowInInspector();
	}
	catch (const std::exception&)
	{

	}
}