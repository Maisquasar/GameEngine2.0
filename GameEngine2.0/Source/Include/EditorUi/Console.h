#pragma once
#include <vector>
#include <string>

#include "EditorWindow.h"
namespace Debug {
	enum class LogType
	{
		INFO,
		WARNING,
		L_ERROR,
	};
}
namespace EditorUi {
	struct ConsoleText
	{
		std::string _text;
		Debug::LogType _type;
		ConsoleText(Debug::LogType c, std::string t)
		{
			_type = c;
			_text = t;
		}
	};

	class Console : public EditorWindow
	{
	public:
		Console();
		~Console();

		void Draw() override;
		void AddLine(Debug::LogType, std::string);
		void Clear();
	private:
		std::vector<ConsoleText> _consoleText;
		std::string _checkBoxTexts[3];
		int _numberOfInfo = 0;
		int _numberOfWarn = 0;
		int _numberOfErro = 0;
		bool _showInfo = true;
		bool _showWarning = true;
		bool _showError = true;
		int _maxLog = 200;

		int GetNumberOfLogType(Debug::LogType);
	};
}