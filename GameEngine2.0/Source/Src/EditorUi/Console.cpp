#include "Include/EditorUi/Console.h"

EditorUi::Console::Console()
{
}

EditorUi::Console::~Console()
{
}

void EditorUi::Console::Draw()
{
	if (!_open)
		return;
	if (ImGui::Begin("Console", &_open))
	{
		// ----------- LogType Buttons ----------- //
		char info[64];
		sprintf_s(info, "%d Info", GetNumberOfLogType(Debug::LogType::INFO));
		ImGui::Checkbox(info, &_showInfo);
		ImGui::SameLine();
		char warning[64];
		sprintf_s(warning, "%d Warning", GetNumberOfLogType(Debug::LogType::WARNING));
		ImGui::Checkbox(warning, &_showWarning);
		ImGui::SameLine();
		char error[64];
		sprintf_s(error, "%d Error", GetNumberOfLogType(Debug::LogType::L_ERROR));	
		ImGui::Checkbox(error, &_showError);

		ImGui::Separator();
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
			for (auto& t : _consoleText)
			{
				switch (t._type)
				{
				case Debug::LogType::INFO:
					if (_showInfo)
						ImGui::Text(t._text.c_str());
					break;
				case Debug::LogType::WARNING:
					if (_showWarning)
						ImGui::TextColored(ImColor(255, 128, 0), t._text.c_str());
					break;
				case Debug::LogType::L_ERROR:
					if (_showError)
						ImGui::TextColored(ImColor(255, 0, 0), t._text.c_str());
					break;
				default:
					break;
				}
			}
			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
			ImGui::EndChild();
		}
		ImGui::Separator();
		static char input[64];
		ImGui::InputText("Input", input, 64, ImGuiInputTextFlags_EnterReturnsTrue);
	}
	ImGui::End();
}

void EditorUi::Console::AddLine(Debug::LogType t, std::string s)
{
	auto Text = ConsoleText{ t, s };
	_consoleText.push_back(Text);
}

int EditorUi::Console::GetNumberOfLogType(Debug::LogType t)
{
	int n = 0;
	for (auto i : this->_consoleText)
	{
		if (i._type == t)
			n++;
	}
	return n;
}
