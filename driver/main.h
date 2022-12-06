#pragma once
#include <string>
#include <mutex>
#include <iostream>

namespace driver
{
	shared_mem_t pac;
	std::mutex mtx;
	HANDLE kernel_event, user_event;
	int timeout = INFINITE;

	void wipe_struct()
	{
		pac = {};
		pac.status = STATUS_UNSUCCESSFUL;
	}

	void exit()
	{

		mtx.lock();
		wipe_struct();

		pac.opcode = close;

		SetEvent(user_event);

		Sleep(1000);

		mtx.unlock();
		ExitProcess(0);
	}

	void expect_answer()
	{
		SetEvent(user_event);
		if (WaitForSingleObject(kernel_event, timeout) == WAIT_ABANDONED)
		{
			driver::exit();
		}
	}
	bool is_running()
	{
		bool ret = false;

		mtx.lock();
		wipe_struct();

		pac.opcode = signal;

		SetEvent(user_event);
		ret = (WaitForSingleObject(kernel_event, 2000) != WAIT_ABANDONED);

		mtx.unlock();
		return ret;
	}

	template <typename T>
	T rpm(uint64_t addy)
	{
		if (!addy || addy < 0x1000000 || addy > 0x7FFFFFFFFFFF) return {};

		NTSTATUS status = STATUS_UNSUCCESSFUL;

		mtx.lock();
		wipe_struct();

		T buffer = {};

		pac.ptr1 = (PVOID)addy;
		pac.ptr2 = (PVOID)&buffer;
		pac.int1 = sizeof(T);
		pac.opcode = read;

		expect_answer();
		status = pac.status;
		mtx.unlock();

		if (NT_SUCCESS(status))
			return buffer;
		else
			return {};
	}

	bool rpm_raw(uint64_t addy, uintptr_t buffer, size_t size)
	{
		if (!addy || addy < 0x1000000 || addy > 0x7FFFFFFFFFFF) return {};

		NTSTATUS status = STATUS_UNSUCCESSFUL;

		mtx.lock();
		wipe_struct();

		pac.ptr1 = (PVOID)addy;
		pac.ptr2 = (PVOID)buffer;
		pac.int1 = size;
		pac.opcode = read;

		expect_answer();
		status = pac.status;
		mtx.unlock();

		return NT_SUCCESS(status);
	}

	bool wpm_raw(uint64_t addy, uintptr_t buffer, size_t size)
	{
		if (!addy || addy < 0x1000000 || addy > 0x7FFFFFFFFFFF) return {};

		NTSTATUS status = STATUS_UNSUCCESSFUL;

		mtx.lock();
		wipe_struct();

		pac.ptr1 = (PVOID)buffer;
		pac.ptr2 = (PVOID)addy;
		pac.int1 = size;
		pac.opcode = write;

		expect_answer();
		status = pac.status;
		mtx.unlock();

		return NT_SUCCESS(status);
	}


	template <typename T>
	bool wpm(T data, uint64_t addy)
	{
		if (!addy || addy < 0x1000000 || addy > 0x7FFFFFFFFFFF) return {};

		NTSTATUS status = STATUS_UNSUCCESSFUL;

		mtx.lock();
		wipe_struct();

		pac.ptr1 = (PVOID)&data;
		pac.ptr2 = (PVOID)addy;
		pac.int1 = sizeof(T);
		pac.opcode = write;

		expect_answer();
		status = pac.status;
		mtx.unlock();

		return NT_SUCCESS(status);

	}

	bool set_process(uint32_t pid)
	{

		NTSTATUS status = STATUS_UNSUCCESSFUL;

		mtx.lock();
		wipe_struct();

		pac.int1 = pid;
		pac.opcode = set_target;

		expect_answer();
		status = pac.status;
		mtx.unlock();

		return NT_SUCCESS(status);
	}

	uintptr_t get_mod_base(std::string name)
	{
		NTSTATUS status = STATUS_UNSUCCESSFUL;
		uintptr_t var = 0;

		mtx.lock();
		wipe_struct();

		std::wstring wstr{ name.begin(), name.end() };
		wcscpy(pac.str, wstr.c_str());

		pac.ptr1 = (PVOID)&var;
		pac.opcode = module_address;

		expect_answer();
		status = pac.status;
		mtx.unlock();

		if (NT_SUCCESS(status))
			return var;
		else
			return 0;

	}


	bool init()
	{
		LoadLibraryA(xorstr("user32.dll"));
		uintptr_t function = (uintptr_t)GetProcAddress(LoadLibraryA(xorstr("win32u.dll")), xorstr("NtTokenManagerCreateCompositionTokenHandle"));

		shared_init_t* funcParam = new shared_init_t();
		funcParam->pid = GetCurrentProcessId();
		funcParam->address = &pac;

		std::wstring user_event_str;
		if (!utils::create_rnd_event(user_event_str, &user_event)) return false;

		std::wstring kernel_event_str;
		if (!utils::create_rnd_event(kernel_event_str, &kernel_event)) return false;

		memcpy(funcParam->user_event, user_event_str.c_str(), sizeof(shared_init_t::user_event));
		memcpy(funcParam->kernel_event, kernel_event_str.c_str(), sizeof(shared_init_t::kernel_event));

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)function, funcParam, 0, 0);

		if (!is_running()) return 0;

		return 1;
	}

}