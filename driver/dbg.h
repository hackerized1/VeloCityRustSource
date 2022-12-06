#pragma once
#include <string>
#include <mutex>
#include <iostream>

namespace driver
{
	HANDLE handle = 0;
	DWORD target_pid = 0;

	void exit()
	{
		ExitProcess(0);
	}

	bool is_running()
	{
		return true;
	}

	template <typename T>
	T rpm(uint64_t addy)
	{
		T data;
		ReadProcessMemory(handle, (PVOID)addy, &data, sizeof(T), 0);
		return data;

	}


	bool rpm_raw(uint64_t addy, uintptr_t buffer, size_t size)
	{
		return ReadProcessMemory(handle, (PVOID)addy, (PVOID)buffer, size, 0);
	}


	template <typename T>
	bool wpm(T data, uint64_t addy)
	{

		return WriteProcessMemory(handle, (PVOID)addy, &data, sizeof(T), 0);
	}

	bool wpm_raw(uint64_t addy, uintptr_t buffer, size_t size)
	{
		return WriteProcessMemory(handle, (PVOID)addy, (LPCVOID)buffer, size, 0);
	}


	bool set_process(uint32_t pid)
	{
		target_pid = pid;
		handle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

		return (handle) ? true : false;
	}

	uintptr_t get_mod_base(std::string name)
	{
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, target_pid);
		if (!hSnapShot)  return 0;

		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnapShot, &mEntry))
		{
			if (!strcmp(name.c_str(), mEntry.szModule))
			{
				CloseHandle(hSnapShot);
				return (uintptr_t)mEntry.modBaseAddr;
			}
		}

		while (Module32Next(hSnapShot, &mEntry))
		{
			if (!strcmp(name.c_str(), mEntry.szModule))
			{
				CloseHandle(hSnapShot);
				return (uintptr_t)mEntry.modBaseAddr;
			}
		}
		CloseHandle(hSnapShot);
		return false;

	}


	bool init()
	{
		return 1;
	}


}