#pragma once
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include "Include/EditorUi/Console.h"

namespace Debug {

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
			Console.AddLine(type, buf);
			printf(((std::string)buf + "\n").c_str());
			SetConsoleTextAttribute(hConsole, 15);
		}
		static EditorUi::Console Console;
	private:

	};

}


#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define LOG(t, x, ...) Debug::Log::Print(__FILENAME__, __LINE__, t, x, __VA_ARGS__);
#define PrintLog(x, ...) Debug::Log::Print(__FILENAME__, __LINE__, Debug::LogType::INFO, x, __VA_ARGS__);
#define PrintWarning(x, ...) Debug::Log::Print(__FILENAME__, __LINE__, Debug::LogType::WARNING, x, __VA_ARGS__);
#define PrintError(x, ...) Debug::Log::Print(__FILENAME__, __LINE__, Debug::LogType::WARNING, x, __VA_ARGS__);
#define Serialize(Variable) (#Variable)
#define Cast(T,x) dynamic_cast<T*>(x)
#define MULTITHREAD_LOADING 1