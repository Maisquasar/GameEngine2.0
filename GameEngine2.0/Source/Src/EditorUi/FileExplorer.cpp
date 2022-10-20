#include "Include/EditorUi/FileExplorer.h"

// -------------- File --------------
EditorUi::File::File() {}
EditorUi::File::File(std::string filename) {
	Directory = filename;
	this->Name = filename.substr(filename.find_last_of("//\\") + 1);

	if (this->Name.substr(this->Name.find_last_of(".") + 1) == "png")
	{
		this->Type = EditorUi::FileType::Img;
		if (auto tex = Resources::ResourceManager::Get<Resources::Texture>(this->Directory.c_str())) {
			this->Icon = tex;
			this->ResourceLink = tex;
		}
	}
	else
	{
		this->Type = EditorUi::FileType::Folder;
		if (auto tex = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/folder_icon.png"))
			this->Icon = tex;
		FoundChildren();
	}
	
}

EditorUi::File::~File() {}

void EditorUi::File::FoundChildren()
{
	for (const auto& entry : std::filesystem::directory_iterator(this->Directory)) {
		std::string dir = entry.path().generic_string().data();
		Children.push_back(new File(dir));
	}
}

// -------------- File Explorer --------------

EditorUi::FileExplorer::FileExplorer()
{
	_open = false;
}

EditorUi::FileExplorer::~FileExplorer() {}

void EditorUi::FileExplorer::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("File Explorer", &_open))
	{
		int index = 0;
		for (auto f : _current->Children) {
			ImGui::PushID(index++);
			ImGui::BeginGroup();
			ImGui::ImageButton((ImTextureID)f->Icon->GetData(), ImVec2(86, 86));
			ImGui::Text(f->Name.c_str());
			ImGui::EndGroup();

			auto windowSize = ImGui::GetWindowSize();
			int rounded = round(windowSize.x / 105);
			if (rounded > 0) {
				if (index % rounded != rounded - 1)
				{
					ImGui::SameLine();
				}
			}
			ImGui::PopID();
		}
		ImGui::End();
	}
}

void EditorUi::FileExplorer::SetOpen(bool value)
{
	_open = value;
	this->Refresh();
}

void EditorUi::FileExplorer::Refresh()
{
	_main = new File(_path);
	_current = _main;
}