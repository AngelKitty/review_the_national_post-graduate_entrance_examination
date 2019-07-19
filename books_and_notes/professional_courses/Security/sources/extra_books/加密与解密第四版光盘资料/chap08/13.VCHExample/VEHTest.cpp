/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// VEHTest.cpp : 定义控制台应用程序的入口点。
// Author: achillis
// 编译环境：需要在Vista以后的SDK环境下编译
// 建议运行平台: Vista以上
// 本程序为演示向量化异常处理VCH


#include "stdafx.h"
#include <windows.h>

LONG CALLBACK VectoredExceptionHandler(
	PEXCEPTION_POINTERS ExceptionInfo
	);

LONG CALLBACK VectoredContinueHandler(
	PEXCEPTION_POINTERS ExceptionInfo
	);

DWORD g_dwValue = 0 ;
int _tmain(int argc, _TCHAR* argv[])
{
	PVOID pVehHandle = AddVectoredExceptionHandler(1,VectoredExceptionHandler);
	PVOID pVchHandle = AddVectoredContinueHandler(1,VectoredContinueHandler);

	__try{
		__asm
		{
			;mov dword ptr fs:[0],-1  //如果加上这一句，破坏SEH结构，将会跳过SEH直接执行VCH
			xor eax,eax
			mov [eax],5	//引发内存访问异常
		}
	}
	__except(printf("[SEH] In SEH filter.\n"),EXCEPTION_CONTINUE_EXECUTION) //不处理异常但返回Continue,给VCH执行的机会，读者可以尝试修改这个返回值以观察不同的效果
	{
		printf("[SEH] In SEH Handler.\n");
	}
	printf("After SEH.\n");
	return 0;
}

LONG CALLBACK VectoredExceptionHandler(
	PEXCEPTION_POINTERS ExceptionInfo
	)
{
	printf("[VEH] In VectoredExceptionHandler.\n");
	return EXCEPTION_CONTINUE_SEARCH; //VEH中不处理异常，使异常到达SEH中
}

LONG CALLBACK VectoredContinueHandler(
	PEXCEPTION_POINTERS ExceptionInfo
	)
{
	printf("[VCH] In VectoredContinueHandler.\n");
	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{
		ExceptionInfo->ContextRecord->Eax = (DWORD)&g_dwValue;
		printf("[VCH] Exception Fixed.\n");
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

