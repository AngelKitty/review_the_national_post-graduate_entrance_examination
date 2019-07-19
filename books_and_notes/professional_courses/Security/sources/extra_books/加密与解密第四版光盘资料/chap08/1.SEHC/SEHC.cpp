/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// SEHC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

#pragma comment(linker,"/Entry:main")
#pragma comment(linker,"/subsystem:Windows")

DWORD dwValue = 100 ;
DWORD dwNewPos = 0 ;
char szCaption[] = "SEH Example";
char szMsgFail[] = "SEH Failed.";
char szMsgSuccess[] = "SEH Success.";

EXCEPTION_DISPOSITION __cdecl _except_handler (
	struct _EXCEPTION_RECORD *ExceptionRecord,
	void * EstablisherFrame,
	struct _CONTEXT *ContextRecord,
	void * DispatcherContext
    )
{
	ContextRecord->Eip = (DWORD)dwNewPos; //使程序从新的位置开始运行，跳过错误的提示
	MessageBox(NULL,szMsgSuccess,szCaption,MB_OK);
	return ExceptionContinueExecution ;
}

//手动安装SEH处理程序
__declspec(naked) void main()
{
	__asm
	{
		push ebp
		mov ebp,esp
		push offset _except_handler
		push dword ptr fs:[0]
		mov dword ptr fs:[0],esp
		lea eax,End
		mov dwNewPos,eax
		xor esi,esi
		mov eax,[esi]
		push MB_OK
		push offset szCaption
		push offset szMsgFail
		xor eax,eax
		push eax
		call dword ptr MessageBoxA
End:
		mov esp,dword ptr fs:[0]
		pop DWORD ptr fs:[0]
		mov esp,ebp
		pop ebp
		retn
	}
}

