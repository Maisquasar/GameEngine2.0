#include "Include/EditorUi/FileExplorer.h"

// -------------- File --------------
EditorUi::File::File() {}

EditorUi::File::File(std::string filename) {
	Directory = filename;
	this->Name = filename.substr(filename.find_last_of("//\\") + 1);

	if (this->Name.substr(this->Name.find_last_of(".") + 1) == "png")
	{
		this->Type = EditorUi::FileType::Img;
		if (auto tex = Resources::ResourceManager::Get<Resources::Texture>(this->Name.c_str())) {
			this->Icon = tex;
			this->ResourceLink = tex;
		}
		else
		{
			this->Icon = Resources::ResourceManager::Get<Resources::Texture>("file_icon.png");
		}
	}
	else if (std::filesystem::is_directory(Directory))
	{
		this->Type = EditorUi::FileType::Folder;
		if (auto tex = Resources::ResourceManager::Get<Resources::Texture>("folder_icon.png"))
			this->Icon = tex;
	}
	else
	{
		this->Type = EditorUi::FileType::None;
		if (auto tex = Resources::ResourceManager::Get<Resources::Texture>("file_icon.png"))
			this->Icon = tex;
	}
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


// -------------- File Explorer --------------

EditorUi::FileExplorer::FileExplorer()
{
	_open = false;
	_path = std::filesystem::current_path().string();
	std::replace(_path.begin(), _path.end(), '\\', '/');
}

EditorUi::FileExplorer::~FileExplorer() {}

void EditorUi::FileExplorer::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("File Explorer", &_open))
	{
		// ------------- Back Button ------------- //
		if (ImGui::Button("Back"))
		{
			_current = _current->GetParent();
		}
		ImGui::SameLine();
		// ------------- Directory Input ------------- //
		char Path[2048];
		for (int i = 0; i < _current->Directory.size(); i++)
			Path[i] = _current->Directory[i];
		Path[_current->Directory.size()] = '\0';
		if (ImGui::InputText("Path", Path, 2048, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			try
			{
				std::shared_ptr<File> tmp = std::make_shared<File>(Path);
				tmp->FoundChildren();
				_current = tmp;
			}
			catch (const std::exception& e)
			{
				// Case when not found
				DebugLog(Debug::LogType::L_ERROR, "Error opening folder %s : %s", Path, e.what());
			}
		}
		ImGui::SameLine();
		// ------------- Refresh Button ------------- //
		if (ImGui::Button("Refresh"))
		{
			try
			{
				_current->FoundChildren();
			}
			catch (const std::exception& e)
			{
				DebugLog(Debug::LogType::L_ERROR, "Error opening folder %s : %s", Path, e.what());
				_current = _main;
			}
		}
		ImGui::SameLine();
		static ImGuiTextFilter filter;
		filter.Draw();
		std::vector<std::string> items;
		// ------------- Files Buttons ------------- //
		ImGui::Separator();
		if (ImGui::BeginChild("Child")) {
			int index = 0;
			for (auto& f : _current->Children) {
				items.push_back(f->Name);
				ImGui::PushID(index);
				ImGui::BeginGroup();
				if (filter.PassFilter(items[index].c_str())) {
					if (ImGui::ImageButton((void*)f->Icon->GetData(), ImVec2(86, 86))) {
						try
						{
							if (f->Type == FileType::Folder) {
								f->FoundChildren();
								_current = f;
								ImGui::EndGroup();
								ImGui::PopID();
								break;
							}
						}
						catch (const std::exception& e)
						{
							// Case when not accessible because of rights.
							DebugLog(Debug::LogType::L_ERROR, "Error opening folder %s : %s", f->Directory.c_str(), e.what());
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
				}
				ImGui::EndGroup();

				auto windowSize = ImGui::GetWindowSize();
				int rounded = (int)round(windowSize.x / 105);
				if (rounded > 0) {
					if (index % rounded != rounded - 1)
					{
						ImGui::SameLine();
					}
				}
				index++;
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
	_main = std::make_shared<File>(_path);
	_main->FoundChildren();
	_current = _main;
}