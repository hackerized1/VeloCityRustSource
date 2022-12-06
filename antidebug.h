#pragma once
#include "utils.h"
#include "log.h"
#include "hwid.h"
#include "imports.h"

namespace antidebug
{
	bool passedCreation = false;

	const std::vector<std::string> tools =
	{
	xorstr("joeboxcontrol.exe"),
	xorstr("LordPE.exe"),xorstr("ImportREC.exe"),xorstr("autorunsc.exe"),
	xorstr("cheatengine-i386.exe"),
	xorstr("x64dbg.exe"),xorstr("netmon.exe"),xorstr("HTTPDebuggerSvc.exe"),
	xorstr("HTTPDebuggerUI.exe"),xorstr("PETools.exe"),xorstr("Nemesis.exe"),xorstr("SysInspector.exe"),
	xorstr("ida64.exe"),xorstr("KsDumperClient.exe"),xorstr("regmon.exe"),xorstr("Fiddler.exe"),
	xorstr("tcpview.exe"),xorstr("dumpcap.exe"),xorstr("ResourceHacker.exe"),xorstr("windbg.exe"),xorstr("idaq64.exe"),
	xorstr("procexp.exe"),xorstr("Wireshark.exe"),xorstr("joeboxserver.exe"),xorstr("autoruns.exe"),xorstr("procmon.exe"),xorstr("KsDumper.exe"),
	xorstr("sysAnalyzer.exe"),xorstr("x32dbg.exe"), xorstr("httpdebugger.exe"), xorstr("ImmunityDebugger.exe"),
	xorstr("joeboxserver.exe"),xorstr("ProcessHacker.exe"),
	xorstr("HookExplorer.exe"),xorstr("ida.exe"), xorstr("filemon.exe"),
	xorstr("ollydbg.exe"),xorstr("idaq.exe"),xorstr("sniff_hit.exe"),xorstr("proc_analyzer.exe"), xorstr("cheatengine-x86_64.exe")
	};


	VOID hideThread(HANDLE thread)
	{
		VM_DOLPHIN_BLACK_START
		NtSetInformationThread_t NtSetInformationThread = (NtSetInformationThread_t)GetProcAddress(LoadLibrary(xorstr("ntdll.dll")), xorstr("NtSetInformationThread"));
		NtSetInformationThread(thread, 0x11, 0, 0);
		CloseHandle(thread);
		VM_DOLPHIN_BLACK_END
	}

	VOID doBluescreen()
	{
		VM_DOLPHIN_BLACK_START
#ifdef NDEBUG
		BOOLEAN bl;
		RtlAdjustPrivilege(19, 1, 0, &bl);
		ULONG response;
		NtRaiseHardError(STATUS_HIVE_UNLOADED, 0, 0, 0, 6, &response);
		exit(0); // incase they temp hook the raiseharderror
#endif
		VM_DOLPHIN_BLACK_END
	}


	BOOL ntGlobalDbgFlag()
	{
		VM_DOLPHIN_BLACK_START
			PDWORD pNtGlobalFlag = (PDWORD)(__readgsqword(0x60) + 0xBC);

		BOOL normalDetected = pNtGlobalFlag && *pNtGlobalFlag & 0x00000070;
		if (normalDetected) return 1;

		return 0;
		VM_DOLPHIN_BLACK_END
	}

	BOOL pebDbgFlag()
	{
		VM_DOLPHIN_BLACK_START
			PPEB pPeb = (PPEB)__readgsqword(0x60);
		return pPeb->BeingDebugged == 1;
		VM_DOLPHIN_BLACK_END
	}

	BOOL queryProcessInfoDbgFlag()
	{
		VM_DOLPHIN_BLACK_START
			NtQueryInformationProcess_t NtQueryInformationProcess = (NtQueryInformationProcess_t)GetProcAddress(LoadLibrary(xorstr("ntdll.dll")), xorstr("NtQueryInformationProcess"));;

		DWORD inheritDbg = 0;

		NTSTATUS stat = NtQueryInformationProcess(GetCurrentProcess(), (PROCESS_INFORMATION_CLASS)0x1f, &inheritDbg, sizeof(DWORD), 0);
		if (!stat && !inheritDbg) return 1;

		return 0;
		VM_DOLPHIN_BLACK_END
	}

	VOID run()
	{
		VM_DOLPHIN_BLACK_START
			passedCreation = 1;

		while (1)
		{
			if (ntGlobalDbgFlag() || ntGlobalDbgFlag() || queryProcessInfoDbgFlag())
			{
				doBluescreen();
			}

			for (const auto& tool : tools)
			{
				if (utils::process::get_id(tool.c_str()) > 300)
				{
					doBluescreen();
				}
			}

			Sleep(5000);
		}
		VM_DOLPHIN_BLACK_END
	}
}