#pragma once
#include <stdio.h>
#include <iostream>
#include <Windows.h>

namespace Debug {

	enum class LogType
	{
		INFO,
		WARNING,
		L_ERROR,
	};

	class Log
	{
	public:
		Log();
		~Log();
		template <typename ...Args> static void Print(const char* file, int line, LogType type, const char* format, Args ... args)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			char buf[256];
			char buf2[256];
			sprintf_s(buf, "%s[%d] : ", file, line);
			sprintf_s(buf2, format, args ...);
			strcat_s(buf, buf2);
			switch (type)
			{
			case Debug::LogType::INFO:
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case Debug::LogType::WARNING:
				SetConsoleTextAttribute(hConsole, 14);
				break;
			case Debug::LogType::L_ERROR:
				SetConsoleTextAttribute(hConsole, 4);
				break;
			default:
				break;
			}
			printf(buf);
			printf("\n");
			SetConsoleTextAttribute(hConsole, 15);
		}

	private:

	};

}

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DebugLog(t, x, ...) Debug::Log::Print(__FILENAME__, __LINE__, t, x, __VA_ARGS__);