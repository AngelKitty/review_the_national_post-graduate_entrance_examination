// SafeSEH.cpp : 定义控制台应用程序的入口点。
// Author:achillis
// 本程序为演示SafeSEH安全设置对异常处理的影响
// 请使用支持SafeSEH设置的编译器（VS2003以上）在打开或关闭SafeSEH开关的情况下编译本程序，观察不同的效果
/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/
#include "stdafx.h"
#include <windows.h>
#include <ImageHlp.h>

#pragma comment(lib,"imagehlp.lib")

typedef struct {
	DWORD       Size;
	DWORD       TimeDateStamp;
	WORD        MajorVersion;
	WORD        MinorVersion;
	DWORD       GlobalFlagsClear;
	DWORD       GlobalFlagsSet;
	DWORD       CriticalSectionDefaultTimeout;
	DWORD       DeCommitFreeBlockThreshold;
	DWORD       DeCommitTotalFreeThreshold;
	DWORD       LockPrefixTable;            // VA
	DWORD       MaximumAllocationSize;
	DWORD       VirtualMemoryThreshold;
	DWORD       ProcessHeapFlags;
	DWORD       ProcessAffinityMask;
	WORD        CSDVersion;
	WORD        Reserved1;
	DWORD       EditList;                   // VA
	DWORD_PTR   *SecurityCookie;
	PVOID       *SEHandlerTable;
	DWORD       SEHandlerCount;
} IMAGE_LOAD_CONFIG_DIRECTORY32_2;

VOID ShowSafeSEHHandler(HMODULE hMod);
VOID SEHSafe();
void SEHUnSafe();

DWORD g_Value = 0;

EXCEPTION_DISPOSITION __cdecl MyHandler (
	struct _EXCEPTION_RECORD *ExceptionRecord,
	void * EstablisherFrame,
	struct _CONTEXT *ContextRecord,
	void * DispatcherContext
	);

int _tmain(int argc, _TCHAR* argv[])
{
	ShowSafeSEHHandler(GetModuleHandle(NULL));
	printf("MyHandler Address = 0x%p\n",(ULONG)MyHandler);
	//测试SEH异常处理程序是否有效
	SEHSafe();
	SEHUnSafe();
	printf("[main] After SEH Function.\n");
	return 0;
}

//在启用了SafeSEH设置后，以下异常处理过程仍然可以正常运行
VOID SEHSafe()
{
	int *pValue = NULL;
	__try
	{
		//引发一个内存写入异常
		*pValue = 5;
	}
	__except(printf("[SEHSafe] In Filter.\n"),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("[SEHSafe] In Handler.\n");
	}
}

//在启用了SafeSEH设置后，以下异常处理过程将无法正常运行
__declspec(naked) void SEHUnSafe()
{
	__asm
	{
		push ebp
		mov ebp,esp
		push offset MyHandler
		push dword ptr fs:[0]
		mov dword ptr fs:[0],esp
		xor eax,eax
		mov eax,[eax]	;引发一个内存读取异常
		mov esp,dword ptr fs:[0]
		pop dword ptr fs:[0]
		mov esp,ebp
		pop ebp
		retn
	}
}

EXCEPTION_DISPOSITION __cdecl MyHandler (
	struct _EXCEPTION_RECORD *ExceptionRecord,
	void * EstablisherFrame,
	struct _CONTEXT *ContextRecord,
	void * DispatcherContext
	)
{
	printf("[SEHUnSafe] In Handler.\n");
	if (ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{
		ContextRecord->Eax = (ULONG)&g_Value;
		printf("[SEHUnSafe] Exception Fixed.\n");
		return ExceptionContinueExecution;
	}
	return ExceptionContinueSearch;
}

//显示SafeSEH表格的内容
VOID ShowSafeSEHHandler(HMODULE hMod)
{
	IMAGE_LOAD_CONFIG_DIRECTORY32_2 *pLoadCfg = NULL ;
	ULONG DirectorySize = 0 ;
	DWORD i = 0 ;

	pLoadCfg = (IMAGE_LOAD_CONFIG_DIRECTORY32_2*)ImageDirectoryEntryToData(
		hMod,TRUE,IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG,&DirectorySize);

	if (DirectorySize == 0)
	{
		printf("No SafeSEH Handler Table.\n");
		return ;
	}

	printf("LoadConfigDir   = 0x%p\n",pLoadCfg);
	printf("Size	        = 0x%X\n",pLoadCfg->Size);
	printf("LockPrefixTable = 0x%p\n",pLoadCfg->LockPrefixTable);
	printf("SecurityCookie  = 0x%p  Cookie = 0x%p\n",pLoadCfg->SecurityCookie,*pLoadCfg->SecurityCookie);
	printf("SEHandlerTable  = 0x%p\n",pLoadCfg->SEHandlerTable);
	printf("SEHandlerCount  = %d\n",pLoadCfg->SEHandlerCount);

	printf("SehHandler Table Details :\n");
	for (i = 0 ;i < pLoadCfg->SEHandlerCount;i++)
	{
		printf("[%d] Handler : 0x%p\n",i,(ULONG_PTR)hMod + (ULONG_PTR)pLoadCfg->SEHandlerTable[i]) ;
	}
	printf("===================Dump SafeSEH Handler Finished.======================\n");

}
