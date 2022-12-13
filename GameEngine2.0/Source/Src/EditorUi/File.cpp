#include "Include/EditorUi/File.h"
#include "Include/App.h"

// -------------- File --------------
EditorUi::File::File() {}

EditorUi::File::File(std::string filename) {
	Directory = filename;
	this->Name = filename.substr(filename.find_last_of("//\\") + 1);
	std::string extension = this->Name.substr(this->Name.find_last_of(".") + 1);
	if (std::filesystem::is_directory(Directory))
	{
		this->Type = EditorUi::FileType::Folder;
		this->Icon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/folder_icon.png");
	}
	else if (extension == "png")
	{
		this->Type = EditorUi::FileType::Img;
		if (auto tex = Application.GetResourceManager()->Get<Resources::Texture>(this->Directory.c_str())) {
			this->Icon = tex;
			this->ResourceLink = tex;
		}
	}
	else if (extension == "mat")
	{
		this->Type = EditorUi::FileType::Mat;
		if (auto mat = Application.GetResourceManager()->Get<Resources::Material>(this->Directory.c_str())) {
			this->Icon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/material_icon.png");
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
		this->Icon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/mtl_icon.png");
	}
	else if (extension == "obj")
	{
		this->Type = EditorUi::FileType::Obj;
		this->Icon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/obj_icon.png");
	}
	else if (extension == "glsl")
	{
		this->Type = EditorUi::FileType::Shdr;
	}
	else
	{
		this->Type = EditorUi::FileType::None;
		this->Icon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/file_icon.png");
	}
	if (!Icon)
		this->Icon = Application.GetResourceManager()->Get<Resources::Texture>("Assets/Default/Textures/file_icon.png");
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

void EditorUi::File::FindAllChilden()
{
	Children.clear();
	if (Directory.substr(this->Directory.find_last_of("/") + 1) == this->Directory)
		this->Directory += '/';
	if (this->Type == FileType::Folder) {
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
		for (auto child : Children)
		{
			child->FindAllChilden();
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
void EditorUi::File::DrawInFileExplorer(File* &clicked)
{
	if (Type == FileType::Folder)
	{
		auto flag = ImGuiTreeNodeFlags_Leaf;
		for (auto child : Children) {
			if (child->Type == FileType::Folder) {
				flag = ImGuiTreeNodeFlags_None;
				break;
			}
		}
		ImGui::BeginGroup();
		ImGui::Image((ImTextureID)static_cast<uintptr_t>(Icon->GetData()), ImVec2(16, 16));
		ImGui::SameLine();
		if (ImGui::TreeNodeEx(this->Name.c_str(), flag)) {
			if (ImGui::IsItemClicked())
			{
				clicked = this;
			}
			for (auto child : Children)
			{
				child->DrawInFileExplorer(clicked);
			}
			ImGui::TreePop();
		}
		ImGui::EndGroup();
	}
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