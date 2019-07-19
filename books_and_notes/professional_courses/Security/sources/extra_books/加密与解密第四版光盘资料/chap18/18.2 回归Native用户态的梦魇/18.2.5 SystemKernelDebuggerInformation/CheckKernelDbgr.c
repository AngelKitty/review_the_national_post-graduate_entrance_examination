/*-----------------------------------------------------------------------
第18章  反跟踪技术
code by forgot 
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/



#include <windows.h>
#include <stdio.h>

#define SystemKernelDebuggerInformation 35

#pragma pack(4)

typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION 
{ 
    BOOLEAN DebuggerEnabled; 
    BOOLEAN DebuggerNotPresent; 
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

typedef DWORD (WINAPI *ZW_QUERY_SYSTEM_INFORMATION)(DWORD, PVOID, ULONG, PULONG);

BOOL
CheckKernelDbgr(
	VOID)
{
	HINSTANCE hModule = GetModuleHandleA("Ntdll");
	ZW_QUERY_SYSTEM_INFORMATION ZwQuerySystemInformation = 
		(ZW_QUERY_SYSTEM_INFORMATION)GetProcAddress(hModule, "ZwQuerySystemInformation");
	SYSTEM_KERNEL_DEBUGGER_INFORMATION Info = {0};

	ZwQuerySystemInformation(
		SystemKernelDebuggerInformation,
		&Info, 
		sizeof(Info),
		NULL);

	return (Info.DebuggerEnabled && !Info.DebuggerNotPresent);
}
