#include "Include/EditorUi/Console.h"
#include "Include/Utils/Loader.h"
#include "Include/App.h"

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
		ImGui::PushID(0);
		// ----------- LogType Buttons ----------- //
		ImGui::Checkbox(_checkBoxTexts[0].c_str(), &_showInfo);
		ImGui::SameLine();

		ImGui::PushStyleColor(0, ImVec4(1, 0.5, 0, 1));
		ImGui::Checkbox(_checkBoxTexts[1].c_str(), &_showWarning);
		ImGui::PopStyleColor();
		ImGui::SameLine();

		ImGui::PushStyleColor(0, ImVec4(1, 0, 0, 1));
		ImGui::Checkbox(_checkBoxTexts[2].c_str(), &_showError);
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::PopID();

		if (ImGui::Button("Clear"))
		{
			Clear();
		}
		ImGui::Separator();

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
			for (auto& t : _consoleText)
			{
				switch (t._type)
				{
				case Debug::LogType::INFO:
					if (_showInfo)
						ImGui::TextUnformatted(t._text.c_str());
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
		}
		ImGui::EndChild();
		ImGui::Separator();
		static char input[64];
		ImGui::InputText("Input", input, 64, ImGuiInputTextFlags_EnterReturnsTrue);
	}
	ImGui::End();
}

void EditorUi::Console::AddLine(Debug::LogType t, std::string s)
{
	Application.ThreadManager.Lock();
	auto Text = ConsoleText{ t, s };

	switch (t)
	{
	case Debug::LogType::INFO:
		_numberOfInfo++;
		break;
	case Debug::LogType::WARNING:
		_numberOfWarn++;
		break;
	case Debug::LogType::L_ERROR:
		_numberOfErro++;
		break;
	}
	if (_consoleText.size() >= _maxLog)
	{
		auto it = _consoleText.begin() + (_consoleText.size() + 1 - _maxLog);
		std::vector<EditorUi::ConsoleText> destroyedText(_consoleText.begin(), it);
		for (auto text : destroyedText)
		{
			switch (text._type)
			{
			case Debug::LogType::INFO:
				--_numberOfInfo;
				break;
			case Debug::LogType::WARNING:
				--_numberOfWarn;
				break;
			case Debug::LogType::L_ERROR:
				--_numberOfErro;
				break;
			default:
				--_numberOfInfo;
				break;
			}
		}
		_consoleText.erase(_consoleText.begin(), it);
	}				
	_checkBoxTexts[0] = Utils::StringFormat("%d Info", GetNumberOfLogType(Debug::LogType::INFO));
	_checkBoxTexts[1] = Utils::StringFormat("%d Warning", GetNumberOfLogType(Debug::LogType::WARNING));
	_checkBoxTexts[2] = Utils::StringFormat("%d Error", GetNumberOfLogType(Debug::LogType::L_ERROR));

	_consoleText.push_back(Text);
	Application.ThreadManager.Unlock();
}

void EditorUi::Console::Clear()
{
	_consoleText.clear();
	_numberOfInfo = 0;
	_numberOfWarn = 0;
	_numberOfErro = 0;
}

int EditorUi::Console::GetNumberOfLogType(Debug::LogType t)
{
	switch (t)
	{
	case Debug::LogType::INFO:
		return _numberOfInfo;
	case Debug::LogType::WARNING:
		return _numberOfWarn;
	case Debug::LogType::L_ERROR:
		return _numberOfErro;
	default:
		return _numberOfInfo;
	}
}