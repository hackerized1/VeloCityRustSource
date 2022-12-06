#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <thread>
#include "xor.h"
#include "math.h"


#define dbg_msg(format, ...) printf( "[call: %s:%s:%d ] " format "\n", __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

namespace utils
{
	namespace process
	{

		int get_id(std::string name)
		{
			PROCESSENTRY32 procEntry32;
			procEntry32.dwSize = sizeof(PROCESSENTRY32);
			HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcSnap == INVALID_HANDLE_VALUE) return 0;

			if (Process32First(hProcSnap, &procEntry32))
			{
				if (!strcmp(name.c_str(), procEntry32.szExeFile))
				{
					return procEntry32.th32ProcessID;
					CloseHandle(hProcSnap);
				}
			}

			while (Process32Next(hProcSnap, &procEntry32))
			{
				if (!strcmp(name.c_str(), procEntry32.szExeFile))
				{
					return procEntry32.th32ProcessID;
					CloseHandle(hProcSnap);
				}
			}

			CloseHandle(hProcSnap);
			return 0;
		}

	}



	namespace window
	{
		HWND wait(std::string name)
		{
			HWND ret;
			while (!IsWindow(ret = FindWindowA(name.c_str(), 0))) Sleep(1000);
			return ret;
		}

		math::vector2 get_size(std::string name)
		{
			RECT rect;
			GetWindowRect(FindWindowA(name.c_str(), 0), &rect);
			return { (float)(rect.right - rect.left), (float)(rect.bottom - rect.top) };
		}
	}


	std::wstring to_wstring(const std::string& as)
	{
		return std::wstring(as.begin(), as.end());
	}

	std::string gen_rnd_str(int length)
	{
		srand(time(0) % rand());

		static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		std::string tmp_s;
		tmp_s.reserve(length);

		for (int i = 0; i < length; ++i) {
			tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
		}

		return tmp_s;
	}

	bool create_rnd_event(std::wstring& name, HANDLE* ret)
	{
		std::string event_temp = xorstr("\\BaseNamedObjects\\Global\\") + utils::gen_rnd_str(10);

		*ret = CreateEventA(0, 0, 0, &event_temp[18]);
		if (!*ret) return false;

		name = to_wstring(event_temp);

		return true;
	}

	void create_thread_safe(uintptr_t start, uintptr_t param = 0)
	{
		HANDLE thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)start, (LPVOID)param, 0, 0);
		if (thread)
		{
			CloseHandle(thread);
		}
	}



}