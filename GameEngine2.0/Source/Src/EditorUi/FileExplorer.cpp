#include "Include/EditorUi/FileExplorer.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/EditorUi/Editor.h"

#pragma region Files
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
	if (ResourceLink)
		ResourceLink->ShowInInspector();
}
#pragma endregion

#pragma region Floating File Explorer
// -------------- Floating File Explorer --------------
EditorUi::FloatingFileExplorer::FloatingFileExplorer()
{
	_windowName = "Floating File Explorer";
	_open = false;
	_path = std::filesystem::current_path().string() + "/Assets";
	std::replace(_path.begin(), _path.end(), '\\', '/');
}

EditorUi::FloatingFileExplorer::~FloatingFileExplorer() {}

std::shared_ptr<EditorUi::File> EditorUi::FloatingFileExplorer::DrawAndRead()
{
	if (!_open || GetState() == EditorUi::FileExplorerState::Write)
		return nullptr;
	std::shared_ptr<File> FileReturn = nullptr;
	bool rightclick = false;
	ImGui::PushID(_windowName.c_str());
	ImGuiWindowFlags_ flag = ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin(_windowName.c_str(), &_open, flag))
	{
		ImGui::SetWindowFocus();
		// ------------- Back Button ------------- //
		if (ImGui::Button("Back"))
		{
			auto path = _current->Directory.substr(_current->Directory.find_last_of('/'), _current->Directory.size());
			_current = _current->GetParent();
		}
		ImGui::SameLine();
		// ------------- Directory Input ------------- //
		char Path[513];
		strcpy_s(Path, 512, _current->Directory.c_str());
		if (ImGui::InputText("Path", Path, 512, ImGuiInputTextFlags_EnterReturnsTrue))
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
				LOG(Debug::LogType::L_ERROR, "Error opening folder %s : %s", Path, e.what());
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
						_rightClicked = f;
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
							else
							{
								FileReturn = f;
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
			_rightClicked = nullptr;
		}
		RightClickWindow();
	}
	ImGui::End();
	ImGui::PopID();
	return FileReturn;
}

void EditorUi::FloatingFileExplorer::DrawAndSave(std::string data)
{
	if (!_open || GetState() == EditorUi::FileExplorerState::Read)
		return;
	ImGui::PushID(_windowName.c_str());
	ImGuiWindowFlags_ flag = ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin(_windowName.c_str(), &_open, flag))
	{
		Draw();
		ImGui::Separator();
		static char FileName[513];
		ImGui::InputText("File Name", FileName, 513);
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			std::ofstream _file;
			auto dir = _current->Directory + "/" + FileName + _targetExtension;
			_file.open(dir);
			LOG(Debug::LogType::INFO, "Saving Scene into %s", dir.c_str());
			this->SetOpen(false);
			this->SetTargetExtension("");
			_file.write(data.c_str(), data.size());

		}
	}
	ImGui::End();
	ImGui::PopID();
}

void EditorUi::FloatingFileExplorer::Draw()
{
	ImGui::SetWindowFocus();
	bool rightclick = false;
	// ------------- Back Button ------------- //
	if (ImGui::Button("Back"))
	{
		auto path = _current->Directory.substr(_current->Directory.find_last_of('/'), _current->Directory.size());
		_current = _current->GetParent();
	}
	ImGui::SameLine();
	// ------------- Directory Input ------------- //
	char Path[513];
	strcpy_s(Path, 512, _current->Directory.c_str());
	if (ImGui::InputText("Path", Path, 512, ImGuiInputTextFlags_EnterReturnsTrue))
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
			LOG(Debug::LogType::L_ERROR, "Error opening folder %s : %s", Path, e.what());
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
	ImGui::BeginChild("Child");
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
				_rightClicked = f;
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
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows) && !rightclick)
	{
		ImGui::OpenPopup("RightClick");
		_rightClicked = nullptr;
	}
	RightClickWindow();
}

void EditorUi::FloatingFileExplorer::SetOpen(bool value)
{
	_open = value;
	this->Refresh();
}

void EditorUi::FloatingFileExplorer::SetTargetExtension(std::string ext)
{
	_targetExtension = ext;
}

void EditorUi::FloatingFileExplorer::SetState(FileExplorerState state)
{
	_state = state;
}

void EditorUi::FloatingFileExplorer::Refresh()
{
	_main = std::make_shared<File>(_path);
	_main->FoundChildren();
	_current = _main;
}

EditorUi::FileExplorerState EditorUi::FloatingFileExplorer::GetState()
{
	return _state;
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
		if (ImGui::Button("New Material"))
		{
			//TODO : this.
		}
		if (_rightClicked != nullptr)
		{
			// ------------- Rename ------------- //
			char Name[64];
			for (int i = 0; i < _rightClicked->Name.size(); i++)
				Name[i] = _rightClicked->Name[i];
			Name[_rightClicked->Name.size()] = '\0';
			if (ImGui::InputText("Rename", Name, 64, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				std::filesystem::rename(_rightClicked->Directory, _rightClicked->Directory.substr(0, _rightClicked->Directory.find_last_of('/') + 1) + Name);
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
						std::filesystem::remove_all(_rightClicked->Directory.c_str());
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
#pragma endregion

#pragma region File Explorer
// ================================ File Explorer ================================ //

EditorUi::FileExplorer::FileExplorer()
{
	_windowName = "File Explorer";
	_path = "Assets";
}

EditorUi::FileExplorer::~FileExplorer()
{
}

void EditorUi::FileExplorer::Draw()
{
	if (!_open)
		return;
	bool rightclick = false;
	ImGui::PushID(_windowName.c_str());
	ImGuiWindowFlags_ flag = ImGuiWindowFlags_::ImGuiWindowFlags_None;
	if (ImGui::Begin(_windowName.c_str(), &_open, flag))
	{
		// ------------- Back Button ------------- //
		if (ImGui::Button("Back"))
		{
			auto path = _current->Directory.substr(_current->Directory.find_last_of('/'), _current->Directory.size());
			if (path != "/Assets")
				_current = _current->GetParent();
		}
		ImGui::SameLine();
		// ------------- Directory Input ------------- //
		char Path[513];
		strcpy_s(Path, 512, _current->Directory.c_str());
		if (ImGui::InputText("Path", Path, 512, ImGuiInputTextFlags_EnterReturnsTrue))
		{

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
			int numberOfItemsPassingFilter = 0;
			for (auto& f : _current->Children) {
				items.push_back(f->Name);
				ImGui::PushID(index);
				ImGui::BeginGroup();
				if (filter.PassFilter(items[index].c_str())) {
					ImGui::ImageButton((ImTextureID)static_cast<uintptr_t>(f->Icon->GetData()), ImVec2(86, 86));
					if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
					{
						ImGui::OpenPopup("RightClick");
						_rightClicked = f;
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
					else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && f->Type != FileType::Folder)
					{
						EditorUi::Editor::GetInspector()->SetFileSelected(f.get());
					}
					// Name of the file.
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						ImGui::Text(f->Name.c_str());
						ImGui::EndTooltip();
					}
					ImGui::TextUnformatted(f->Name.substr(0, 10).c_str());
					numberOfItemsPassingFilter++;
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
			_rightClicked = nullptr;
		}
		RightClickWindow();
	}
	ImGui::End();
	ImGui::PopID();
}
#pragma endregion
