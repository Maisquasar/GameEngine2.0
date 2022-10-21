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
		else
		{
			this->Icon = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/file_icon.png");
		}
	}
	else if (std::filesystem::is_directory(Directory))
	{
		this->Type = EditorUi::FileType::Folder;
		if (auto tex = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/folder_icon.png"))
			this->Icon = tex;
	}
	else
	{
		this->Type = EditorUi::FileType::None;
		if (auto tex = Resources::ResourceManager::Get<Resources::Texture>("Assets/Default/Textures/file_icon.png"))
			this->Icon = tex;

	}

}


EditorUi::File::~File() 
{

}

void EditorUi::File::FoundChildren()
{
	Children.clear();
	if (Directory.substr(this->Directory.find_last_of("/") + 1) == this->Directory)
		this->Directory += '/';
	for (const auto& entry : std::filesystem::directory_iterator(this->Directory)) {
		std::string dir = entry.path().generic_string().data();
		try
		{
			Children.push_back(new File(dir));
		}
		catch (const std::exception&)
		{

		}
	}
}

EditorUi::File* EditorUi::File::GetParent()
{
	auto dir = this->Directory.substr(0, this->Directory.find_last_of("/"));
	auto par = new File(dir);
	par->FoundChildren();
	return par;
}


// -------------- File Explorer --------------

EditorUi::FileExplorer::FileExplorer()
{
	_open = false;
	_path = std::filesystem::current_path().string();
	std::replace(_path.begin(), _path.end(), '\\', '/');
}

EditorUi::FileExplorer::~FileExplorer() {
	_path.clear();
	delete _current;
	if (_main != _current)
		delete _main;
}

void EditorUi::FileExplorer::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("File Explorer", &_open))
	{
		if (ImGui::Button("Back"))
		{
			_current = _current->GetParent();
		}
		ImGui::SameLine();
		// Input Text.
		char Path[2048];
		for (int i = 0; i < _current->Directory.size(); i++)
			Path[i] = _current->Directory[i];
		Path[_current->Directory.size()] = '\0';
		ImGui::InputText("Path", Path, 2048);
		ImGui::Separator();
		// Folders Buttons.
		if (ImGui::BeginChild("Child")) {
			int index = 0;
			for (auto f : _current->Children) {
				ImGui::PushID(index++);
				ImGui::BeginGroup();
				if (ImGui::ImageButton((ImTextureID)f->Icon->GetData(), ImVec2(86, 86))) {
					try
					{
						if (f->Type == FileType::Folder) {
							f->FoundChildren();
							_current = f;
						}
					}
					catch (const std::exception&)
					{
						// Case when not accessible because of rights.
						DebugLog(Debug::LogType::L_ERROR, "Error opening folder %s", f->Directory.c_str());
					}
				}
				// Name of the file.
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text(f->Name.c_str());
					ImGui::EndTooltip();
				}
				ImGui::TextUnformatted(f->Name.substr(0, 10).c_str());
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
			ImGui::EndChild();
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
	_main->FoundChildren();
	_current = _main;
}