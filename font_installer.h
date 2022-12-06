#pragma once
//#include <Windows.h>
//#include <cstdio>
//#include <string>
//#include <filesystem>
//namespace fs = std::filesystem;
//
//#pragma warning(disable : 4996);
//
//namespace font_installer
//{
//	bool is_available(const char* name)
//	{
//		//get fontdir
//		const char* t = getenv("windir");
//		std::string fontdir = t;
//		fontdir += "\\Fonts";
//
//		std::string aname = name;
//		aname += ".ttf";
//		std::wstring wname(aname.begin(), aname.end());
//
//		for (const auto& entry : fs::directory_iterator(fontdir.c_str()))
//		{
//			if (wname.compare(entry.path().filename()) == 0)
//			{
//				return true;
//			}
//		}
//
//		return false;
//	}
//
//	bool install(const char* name)
//	{
//		//move the .ttf file to the windir/fonts
//		//add a reg key C:\Users\admin\AppData\Local\Microsoft\Windows\Fonts\fontname.ttf
//
//		if (is_available(name)) return true;
//
//		//get temp path
//		const char* t = getenv("TEMP");
//		std::string filedir = t;
//		filedir += "\\";
//		filedir += name;
//		filedir += ".ttf";
//
//		//get fontdir
//		t = getenv("windir");
//		std::string fontdir = t;
//		fontdir += "\\Fonts";
//
//		char movecmd[128] = { 0 };
//		sprintf_s(movecmd, 128, "move %s %s", filedir.c_str(), fontdir.c_str());
//		system(movecmd);
//
//		char regaddcmd[512] = { 0 };
//		sprintf_s(regaddcmd, 512, "reg add \"HKLM\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts\" /v \"%s (TrueType)\" /d \"%s.ttf\"", name, name);
//		system(regaddcmd);
//
//		return true;
//	}
//}