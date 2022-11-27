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

EditorUi::FloatingFileExplorer::FloatingFileExplorer()
{
	_windowName = "Floating File Explorer";
	_limited = false;
	_open = false;
	_path = std::filesystem::current_path().string() + "/Assets";
	std::replace(_path.begin(), _path.end(), '\\', '/');
}

EditorUi::FloatingFileExplorer::~FloatingFileExplorer() {}

void EditorUi::FloatingFileExplorer::Draw()
{
	if (!_open)
		return;
	bool rightclick = false;
	ImGui::PushID(_windowName.c_str());
	ImGuiWindowFlags_ flag = ImGuiWindowFlags_None;
	if (!_limited)
		flag = ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin(_windowName.c_str(), &_open, flag))
	{
		// ------------- Back Button ------------- //
		if (ImGui::Button("Back"))
		{
			auto path = _current->Directory.substr(_current->Directory.find_last_of('/'), _current->Directory.size());
			if (_limited && path != "/Assets")
				_current = _current->GetParent();
			else if (!_limited)
				_current = _current->GetParent();
		}
		ImGui::SameLine();
		// ------------- Directory Input ------------- //
		char Path[513];
		strcpy_s(Path, 512, _current->Directory.c_str());
		if (ImGui::InputText("Path", Path, 512, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (!_limited) {
				try
				{
					std::shared_ptr<File> tmp = std::make_shared<File>(Path);
					tmp->FoundChildren();
					_current = tmp;
				}
				catch (const std::exception& e)
				{
					// Case when not found
					LOG(Debug::LogType::L_ERROR, "Error opening folder %s : %s", Path, e.what());
				}
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
				LOG(Debug::LogType::L_ERROR, "Error opening folder %s : %s", Path, e.what());
				_current = _main;
			}
		}
		// ------------- Filter Text ------------- //
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
					ImGui::ImageButton((ImTextureID)static_cast<uintptr_t>(f->Icon->GetData()), ImVec2(86, 86));
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
					{
						ImGui::OpenPopup("RightClick");
						_clicked = f;
						rightclick = true;
					}
					RightClickWindow();
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
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
							LOG(Debug::LogType::L_ERROR, "Error opening folder %s : %s", f->Directory.c_str(), e.what());
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
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && !rightclick)
		{
			ImGui::OpenPopup("RightClick");
			_clicked = nullptr;
		}
		RightClickWindow();
	}
	ImGui::End();
	ImGui::PopID();
}

void EditorUi::FloatingFileExplorer::SetOpen(bool value)
{
	_open = value;
	this->Refresh();
}

void EditorUi::FloatingFileExplorer::Refresh()
{
	_main = std::make_shared<File>(_path);
	_main->FoundChildren();
	_current = _main;
}

void EditorUi::FloatingFileExplorer::RightClickWindow()
{
	if (ImGui::BeginPopup("RightClick", ImGuiWindowFlags_NoDecoration))
	{
		if (ImGui::Button("New Folder"))
		{
			std::filesystem::create_directory(_current->Directory + "/NewFolder");
			_current->FoundChildren();
		}
		if (_clicked != nullptr)
		{
			// ------------- Rename ------------- //
			char Name[64];
			for (int i = 0; i < _clicked->Name.size(); i++)
				Name[i] = _clicked->Name[i];
			Name[_clicked->Name.size()] = '\0';
			if (ImGui::InputText("Rename", Name, 64, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::filesystem::rename(_clicked->Directory, _clicked->Directory.substr(0, _clicked->Directory.find_last_of('/') + 1) + Name);
				_current->FoundChildren();
				ImGui::OpenPopup("Rename");
				ImGui::CloseCurrentPopup();
			}
			// ------------- Delete ------------- //
			if (ImGui::Button("Delete"))
			{
				ImGui::OpenPopup("Delete");
			}
			if (ImGui::BeginPopupModal("Delete"))
			{
				ImGui::Text("Are you sure ?");
				if (ImGui::Button("Yes"))
				{
					try
					{
						std::filesystem::remove_all(_clicked->Directory.c_str());
					}
					catch (const std::exception& e)
					{
						LOG(Debug::LogType::L_ERROR, "Error Delete file %s", e.what());
					}
					_current->FoundChildren();
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("No"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
		ImGui::EndPopup();
	}
}

EditorUi::FileExplorer::FileExplorer()
{
	_windowName = "File Explorer";
	_limited = true;
	_path = "Assets";
}

EditorUi::FileExplorer::~FileExplorer()
{
}
