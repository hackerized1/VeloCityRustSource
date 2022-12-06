#pragma once

#include <Windows.h>
#include <ntstatus.h>

#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

#define THREAD_ALL_ACCESS_VISTA         (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
	0xFFFF)

#define THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER 0x00000004



typedef struct _INITIAL_TEB
{
    PVOID                StackBase;
    PVOID                StackLimit;
    PVOID                StackCommit;
    PVOID                StackCommitMax;
    PVOID                StackReserved;
} INITIAL_TEB, * PINITIAL_TEB;

typedef NTSTATUS(NTAPI* NtSetInformationThread_t)(
    _In_ HANDLE ThreadHandle,
    _In_ ULONG  ThreadInformationClass,
    _In_ PVOID  ThreadInformation,
    _In_ ULONG  ThreadInformationLength
    );

typedef NTSTATUS(NTAPI* NtCreateThreadEx_t)(_Out_ PHANDLE ThreadHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
    _In_ HANDLE ProcessHandle,
    _In_ PVOID StartRoutine,
    _In_opt_ PVOID Argument,
    _In_ ULONG CreateFlags,
    _In_opt_ ULONG_PTR ZeroBits,
    _In_opt_ SIZE_T StackSize,
    _In_opt_ SIZE_T MaximumStackSize,
    _In_opt_ PVOID AttributeList);

typedef NTSTATUS(NTAPI* NtQueryInformationProcess_t)(
    IN HANDLE               ProcessHandle,
    IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
    OUT PVOID               ProcessInformation,
    IN ULONG                ProcessInformationLength,
    OUT PULONG              ReturnLength);

extern "C"
{
    //http://undocumented.ntinternals.net/
    NTSYSAPI NTSTATUS NTAPI NtLoadDriver(PUNICODE_STRING);
    NTSYSAPI NTSTATUS NTAPI NtUnloadDriver(PUNICODE_STRING);

    NTSYSAPI NTSTATUS NTAPI RtlAdjustPrivilege(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
    NTSYSAPI NTSTATUS NTAPI NtRaiseHardError(NTSTATUS, ULONG, ULONG, PULONG_PTR, ULONG, PULONG);
}