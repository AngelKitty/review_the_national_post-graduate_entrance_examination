// ShimInjector.cpp : Defines the entry point for the console application.
// 链接：http://bbs.pediy.com/showthread.php?t=175483
// 原作者:山村野人


#include "stdafx.h"
#include <windows.h>

typedef LONG NTSTATUS;

typedef struct _PROCESS_BASIC_INFORMATION{
	NTSTATUS ExitStatus;
	PVOID PebBaseAddress;
	ULONG_PTR AffinityMask;
	LONG BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
}PROCESS_BASIC_INFORMATION,*PPROCESS_BASIC_INFORMATION;

typedef NTSTATUS (NTAPI* fnNtQueryInformationProcess)(HANDLE,ULONG,PVOID,ULONG,PULONG);

#ifdef _WIN64
#define SHIMDATA_OFFSET_IN_PEB (0x2D8) //x64
#else
//WinXp/Win2003/Win7/Win8/Win10偏移都是这个值
//Peb+0x1e8 pShimData        : Ptr32 Void
#define SHIMDATA_OFFSET_IN_PEB (0x1E8) //x86
#endif

int main(int argc, char* argv[])
{
	STARTUPINFOW si = {0};
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;
	PROCESS_BASIC_INFORMATION pbi;
	SIZE_T SizeCnt = 0 ;

#ifdef _WIN64
	wchar_t wExePath[MAX_PATH] = L"F:\\Program2016\\DllInjection\\HostProc64.exe" ;
	wchar_t wDllPath[MAX_PATH] = L"F:\\Program2016\\DllInjection\\14.ShimEngInject\\ShimEngHack\\Bin\\ShimEngHack64.dll";
#else
	wchar_t wExePath[MAX_PATH] = L"F:\\Program2016\\DllInjection\\HostProc.exe" ;
	wchar_t wDllPath[MAX_PATH] = L"F:\\Program2016\\DllInjection\\14.ShimEngInject\\ShimEngHack\\Bin\\ShimEngHack.dll";
#endif
	
	//测试时需要先把要注入的目标程序设置为兼容模式，否则不会启用ShimEngine
	

	CreateProcessW(wExePath,NULL,NULL,NULL,FALSE,CREATE_SUSPENDED | CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi);
	((fnNtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"),"NtQueryInformationProcess"))(pi.hProcess,0,&pbi,sizeof(PROCESS_BASIC_INFORMATION),NULL);
	LPVOID lpShimData = (LPVOID)((ULONG_PTR)pbi.PebBaseAddress + SHIMDATA_OFFSET_IN_PEB);
	PVOID pShimData = NULL;
	ReadProcessMemory(pi.hProcess,lpShimData,&pShimData,sizeof(PVOID),NULL);
	printf("pShimData = 0x%p\n",pShimData);
	if (pShimData) WriteProcessMemory(pi.hProcess,pShimData,wDllPath,lstrlenW(wDllPath) * sizeof(wchar_t),&SizeCnt);
	printf("WriteSize = %d \n",SizeCnt);
	ResumeThread(pi.hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return 0 ;
}
