/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/
// VEHExample.cpp : Defines the entry point for the application.
// Author: achillis
// 建议运行平台: WinXP以上
// 本程序为演示向量化异常处理

#include "stdafx.h"

#define _WIN32_WINNT 0x502
#include <windows.h>

char szTit[]="design : achillis XP+";
DWORD validADDR;

LONG WINAPI vh0(PEXCEPTION_POINTERS ExceptionInfo)
{
    PCONTEXT pContext=ExceptionInfo->ContextRecord;
    pContext->Eax=(DWORD)&validADDR;
    return  EXCEPTION_CONTINUE_EXECUTION;
}


int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR CmdLine,int nCmdShow)
{
    PVOID handle = AddVectoredExceptionHandler(TRUE,vh0);
	
	__asm
	{
		xor eax,eax
		mov [eax],5 //向0地址写入数据，引发内存访问异常
	}
    
    MessageBox(0,"We SUC recovering from Write Acess!",szTit,MB_ICONINFORMATION);
    RemoveVectoredExceptionHandler(handle);
    return 0;
}