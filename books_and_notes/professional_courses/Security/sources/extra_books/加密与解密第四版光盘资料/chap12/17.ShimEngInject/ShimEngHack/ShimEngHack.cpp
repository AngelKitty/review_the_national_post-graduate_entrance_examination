// ShimEngHack.cpp : Defines the entry point for the DLL application.
// 链接：http://bbs.pediy.com/showthread.php?t=175483
// 原作者:山村野人

#include "stdafx.h"

typedef LONG NTSTATUS;
#define STATUS_SUCCESS	(0x00000000L)
#define PLDR_DATA_TABLE_ENTRY PVOID

typedef struct _UNICODE_STRING{
	USHORT Length;
	USHORT MaximumLength;
	PWCH Buffer;
}UNICODE_STRING,*PUNICODE_STRING;

NTSTATUS WINAPI SE_InitializeEngine(PUNICODE_STRING pusCoreDllFile,PUNICODE_STRING pusExecuteFileName,PVOID pShimData)
{
	OutputDebugStringA("SE_InitializeEngine\n");
	return STATUS_SUCCESS;
}

VOID WINAPI SE_InstallBeforeInit(PUNICODE_STRING pusExecuteFileName,PVOID pShimData)
{
	OutputDebugStringA("SE_InstallBeforeInit\n");
}

BOOL WINAPI SE_InstallAfterInit(PUNICODE_STRING pusExecuteFileName,PVOID pShimData)
{
	OutputDebugStringA("SE_InstallAfterInit\n");
	return TRUE;
}

VOID WINAPI SE_DllLoaded(PLDR_DATA_TABLE_ENTRY pLdrModuleLoaded)
{
	OutputDebugStringA("SE_DllLoaded\n");
}

VOID WINAPI SE_DllUnloaded(PLDR_DATA_TABLE_ENTRY pLdrModuleUnload)
{
	OutputDebugStringA("SE_DllUnloaded\n");
}

VOID WINAPI SE_GetProcAddress(PVOID pvUnknown0)
{
}

VOID WINAPI SE_ProcessDying()
{
}

VOID WINAPI SE_ShimDllLoaded(HMODULE hModule)
{
	OutputDebugStringA("SE_ShimDllLoaded\n");
}

VOID WINAPI SE_LdrEntryRemoved(PLDR_DATA_TABLE_ENTRY pLdrEntryRemoved)
{
}

VOID WINAPI SE_LdrResolveDllName(PUNICODE_STRING pusUnknown,PVOID pvModuleDataUnk,PUNICODE_STRING pusModuleFileName)
{
}

VOID WINAPI SE_GetProcAddressLoad(PLDR_DATA_TABLE_ENTRY pLdrEntry)
{
}



VOID WINAPI SE_GetProcAddressForCaller(PVOID pvUnknown0,PVOID pvUnknown1,PVOID pfnCallProcAddr,ULONG_PTR ulZero,PVOID pfnReturnToAddr)
{
}

BOOL WINAPI ApphelpCheckModule(PUNICODE_STRING pusModuleName,PVOID pvUnknown1,PVOID pvUnknown2,PVOID pvUnknown3,PVOID pvUnknown4,PVOID pvUnknown5,PVOID pvUnknown6)
{
	return FALSE;
}

BOOL WINAPI DllMain(HINSTANCE hDllInst,DWORD dwReason,LPVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hDllInst);
#ifdef _WIN64
		LoadLibraryA("F:\\Program2016\\DllInjection\\MsgDll64.dll");
#else
		LoadLibraryA("F:\\Program2016\\DllInjection\\MsgDll.dll");
#endif
	
		OutputDebugStringA("FUN!\n");
	}
	return TRUE;
}