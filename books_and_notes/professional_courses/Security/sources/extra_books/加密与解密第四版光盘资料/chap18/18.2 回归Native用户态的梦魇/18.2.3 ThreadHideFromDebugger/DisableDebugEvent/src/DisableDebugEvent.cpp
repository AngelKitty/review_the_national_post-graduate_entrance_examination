/*-----------------------------------------------------------------------
第18章  反跟踪技术
code by forgot 
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


#include <stdio.h> 

#include <windows.h>
#include <tchar.h>

typedef DWORD (WINAPI *ZW_SET_INFORMATION_THREAD)(HANDLE, DWORD, PVOID, ULONG);
#define ThreadHideFromDebugger 17
VOID DisableDebugEvent(VOID)
{
	HINSTANCE hModule;
	ZW_SET_INFORMATION_THREAD ZwSetInformationThread;

	hModule = GetModuleHandleA("Ntdll");
	ZwSetInformationThread = 
		(ZW_SET_INFORMATION_THREAD)GetProcAddress(hModule, "ZwSetInformationThread");
	ZwSetInformationThread(GetCurrentThread(), ThreadHideFromDebugger, NULL, NULL);
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
DisableDebugEvent();
     return 0 ;
}

