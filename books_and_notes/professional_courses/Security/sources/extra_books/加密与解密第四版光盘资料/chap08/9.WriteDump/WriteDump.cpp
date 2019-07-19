// WriteDump.cpp : Defines the entry point for the console application.
// Author:achillis
// 本程序用于演示顶层异常处理的使用及生成Dump技术
//
/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/
#include "stdafx.h"
#include <WINDOWS.H>
#include <DbgHelp.h>

#pragma comment(lib,"Dbghelp.lib")

LONG WINAPI TopLevelExceptionFilter(
	struct _EXCEPTION_POINTERS* ExceptionInfo
	);

int main(int argc, char* argv[])
{
	//安装顶层异常处理回调
	SetUnhandledExceptionFilter(TopLevelExceptionFilter);
	int *pValue = NULL;
	*pValue = 5 ; //引发内存访问异常
	return 0;
}

LONG WINAPI TopLevelExceptionFilter(
	struct _EXCEPTION_POINTERS* ExceptionInfo
	)
{
	printf("Exception Catched, Code = 0x%08X EIP = 0x%p\n",
		ExceptionInfo->ExceptionRecord->ExceptionCode,
		ExceptionInfo->ExceptionRecord->ExceptionAddress);
	printf(".exr = 0x%p\n",ExceptionInfo->ExceptionRecord);
	printf(".cxr = 0x%p\n",ExceptionInfo->ContextRecord);

	HANDLE hDumpFile = CreateFile("Dump.dmp",
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hDumpFile == INVALID_HANDLE_VALUE)
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	MINIDUMP_EXCEPTION_INFORMATION MinidumpExpInfo;
	ZeroMemory(&MinidumpExpInfo,sizeof(MINIDUMP_EXCEPTION_INFORMATION));
	MinidumpExpInfo.ThreadId = GetCurrentThreadId();
	MinidumpExpInfo.ExceptionPointers = ExceptionInfo;
	MinidumpExpInfo.ClientPointers = TRUE ;

	BOOL bResult = MiniDumpWriteDump(GetCurrentProcess(),
		GetCurrentProcessId(),
		hDumpFile,
		MiniDumpWithProcessThreadData,
		&MinidumpExpInfo,
		NULL,
		NULL
		);
	
	printf("Write Dump File %s .\n",bResult ? "Success":"Failed");
	CloseHandle(hDumpFile);
	
	//Dump文件生成完毕，可以结束进程了
	return EXCEPTION_EXECUTE_HANDLER;
}
