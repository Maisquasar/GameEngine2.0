#include "Include/EditorUi/FloatingFileExplorer.h"
#include "Include/EditorUi/Inspector.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/EditorUi/Editor.h"
#include "Include/Utils/Loader.h"
#include "Include/App.h"

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
								auto ext = f->Name.substr(f->Name.find_last_of('.'));
								if (ext == _targetExtension)
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
		}
		ImGui::EndChild();
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
	static char FileName[513];
	if (!_open || GetState() == EditorUi::FileExplorerState::Read)
		return;
	ImGui::PushID(_windowName.c_str());
	ImGuiWindowFlags_ flag = ImGuiWindowFlags_::ImGuiWindowFlags_NoDocking;
	if (ImGui::Begin(_windowName.c_str(), &_open, flag))
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
				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					strcpy_s(FileName, f->Name.substr(0, f->Name.find_last_of('.')).c_str());
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
		ImGui::Separator();
		ImGui::InputText("File Name", FileName, 513);
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			std::ofstream _file;
			auto dir = _current->Directory + "/" + FileName + _targetExtension;
			_file.open(dir);
			this->SetOpen(false);
			this->SetTargetExtension("");
			_file.write(data.c_str(), data.size());
			LOG(Debug::LogType::INFO, "Saving File into %s", dir.c_str());

		}
		ImGui::SameLine();
		ImGui::Text(_targetExtension.c_str());
	}
	ImGui::End();
	ImGui::PopID();
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
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("New Material"))
		{
			auto mat = new Resources::Material();
			mat->SetPath(_current->Directory + "/NewMaterial.mat");
			mat->SetName("NewMaterial");
			Application.GetResourceManager()->Add(mat->GetPath().c_str(), mat);
			Utils::Loader::WriteMaterial(mat);
			_current->FoundChildren();
			ImGui::CloseCurrentPopup();
		}
		if (_rightClicked != nullptr)
		{
			ImGui::Separator();
			// ------------- Rename ------------- //
			char Name[64];
			strcpy_s(Name, 64, _rightClicked->Name.c_str());
			if (ImGui::InputText("Rename", Name, 64, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				auto NewPath = _rightClicked->Directory.substr(0, _rightClicked->Directory.find_last_of('/') + 1) + Name;
				std::filesystem::rename(_rightClicked->Directory, NewPath);
				// Change Resource Linked key to the new Path.
				if (_rightClicked->ResourceLink)
				{
					std::string NewName = Name;
					NewName = NewName.substr(0, NewName.find_last_of(".") - 1);
					_rightClicked->ResourceLink->SetName(NewName);
					Application.GetResourceManager()->ChangeKey(_rightClicked->ResourceLink->GetPath(), NewPath, _rightClicked->ResourceLink);
					_rightClicked->ResourceLink->SetPath(NewPath);
				}
				_current->FoundChildren();
				ImGui::OpenPopup("Rename");
				ImGui::CloseCurrentPopup();
			}
			switch (_rightClicked->Type)
			{
			case FileType::Shdr:
			{
				if (ImGui::Button("Edit"))
				{
					// Settings Parameters
					EditorUi::Editor::GetTextEditor()->Tabs.push_back(TextEditor());
					EditorUi::Editor::GetTextEditor()->Tabs.back().SetTitle(_rightClicked->Name.c_str());
					EditorUi::Editor::GetTextEditor()->Tabs.back().SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
					EditorUi::Editor::GetTextEditor()->Tabs.back().SetPath(_rightClicked->Directory.c_str());

					// Reading File.
					uint32_t size = 0;
					bool sucess;
					auto text = Utils::Loader::ReadFile(_rightClicked->Directory.c_str(), size, sucess);
					std::string cuted = text;
					cuted = cuted.substr(0, size);
					EditorUi::Editor::GetTextEditor()->Tabs.back().SetText(cuted);
					delete[] text;
				}
				break;
			}
			// ------------- Load Scene ------------- //
			case FileType::Scn:
			{
				if (ImGui::Button("LoadScene"))
				{
					Application.GetScene()->LoadScene(_rightClicked->Directory);
				}
				break;
			}
			default:
				break;
			}
			// ------------- Show in Explorer ------------- //
			if (ImGui::Button("Show in Explorer"))
			{

				auto dir = _rightClicked->Directory;
				dir = dir.substr(0, dir.find_last_of('/'));
				if (_rightClicked->Directory.substr(0, _rightClicked->Directory.find_first_of('/')) == "Assets")
				{
					dir = std::filesystem::current_path().string() + '/' + dir;
				}
				ShellExecuteA(NULL, "open", dir.c_str(), NULL, NULL, SW_SHOWDEFAULT);
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
						if (_rightClicked->ResourceLink)
							delete _rightClicked->ResourceLink;
						_rightClicked->ResourceLink = nullptr;
						_rightClicked = nullptr;
						EditorUi::Inspector::SetFileSelected(nullptr);
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
