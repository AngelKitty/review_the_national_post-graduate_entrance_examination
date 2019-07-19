/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// MsgHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MsgHook.h"
#include <stdio.h>

LRESULT CALLBACK MsgHookProc(
										int code,
										WPARAM wParam,
										LPARAM lParam
	);
extern "C" MSGHOOK_API VOID InstallHook();
extern "C" MSGHOOK_API VOID UnInstallHook();

#pragma data_seg(".Share")
HHOOK g_hHook=NULL; //必须赋初值
HMODULE g_hModule = NULL ;
#pragma data_seg()

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	char szModulePath[MAX_PATH]={0};
	char szBuffer[1024]={0};
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				g_hModule = (HMODULE)hModule;
				GetModuleFileName(NULL,szModulePath,MAX_PATH);
				sprintf(szBuffer,"[MsgHook.dll] Injected into %s\n",szModulePath);
				OutputDebugString(szBuffer);
				break;
			}
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			{
				g_hModule = (HMODULE)hModule;
				GetModuleFileName(NULL,szModulePath,MAX_PATH);
				sprintf(szBuffer,"[MsgHook.dll] Unloaded from %s\n",szModulePath);
				OutputDebugString(szBuffer);
				break;
			}
			break;
    }
    return TRUE;
}

LRESULT CALLBACK MsgHookProc(
	int code,
	WPARAM wParam,
	LPARAM lParam
	)
{
	return CallNextHookEx(g_hHook,code,wParam,lParam);
}

extern "C" MSGHOOK_API VOID InstallHook()
{
	g_hHook = SetWindowsHookEx(WH_CALLWNDPROC,MsgHookProc,g_hModule,0);
}

extern "C" MSGHOOK_API VOID UnInstallHook()
{
	UnhookWindowsHookEx(g_hHook);
}
