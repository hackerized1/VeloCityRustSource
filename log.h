#pragma once
#include <cstdio>
#include <time.h>
#include <string>
#include <iostream>
#include <Windows.h>


#ifndef NDEBUG
//#define Log(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define Log(clr, ...) logger::log(clr, __FUNCTION__, __VA_ARGS__)
#else
#define Log(clr,fmt, ...)
#endif // DEBUG

namespace c
{
	int normal = 7;
	int ok = 10;
	int fail = 4;
	int notice = 9;
	int sus = 13;
}

namespace logger
{
	FILE* f;
	HANDLE hconsole;

	void start()
	{
		//AllocConsole();
		//freopen_s(&f, "CONOUT$", "w", stdout);
		hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void set_output_color(int color)
	{
		SetConsoleTextAttribute(hconsole, color);
	}

#ifndef NDEBUG
	template <typename ... args>
	void log(int clr, std::string function, args ... to_print)
	{
		static time_t rawtime;
		struct tm timeinfo;

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		set_output_color(8);
		printf("[%02i:%02i:%02i] ", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
		set_output_color(7);
		std::cout << "[" << function << "] ";
		set_output_color(clr);
		printf(to_print ...);

		set_output_color(c::normal);
	}
#endif

	void free()
	{
		fclose(f);
		//FreeConsole();
	}
}