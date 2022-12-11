#include "Include/EditorUi/FileExplorer.h"
#include "Include/Resources/ResourceManager.h"
#include "Include/EditorUi/Editor.h"
#include "Include/Utils/Loader.h"
#include "Include/App.h"
#include "Include/EditorUi/Inspector.h"

#pragma region Files
#pragma endregion

#pragma region Floating File Explorer
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
		ImGui::BeginChild("List", ImVec2(ImGui::GetWindowWidth() * 0.1f, 0));
		File* clicked = nullptr;
		_main->DrawInFileExplorer(clicked);
		if (clicked)
		{
			_current = std::make_shared<File>(*clicked);
			_current->FoundChildren();
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical | ImGuiSeparatorFlags_SpanAllColumns);
		ImGui::SameLine();
		ImGui::BeginChild("Main");
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
				// At The End Because if file deleted : crash.
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsItemHovered())
				{
					ImGui::OpenPopup("RightClick");
					_rightClicked = f;
					rightclick = true;
				}
				RightClickWindow();

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
		ImGui::EndChild();
	}
	ImGui::End();
	ImGui::PopID();
}
void EditorUi::FileExplorer::Refresh()
{
	FloatingFileExplorer::Refresh();
	_main->FindAllChilden();
}
#pragma endregion
