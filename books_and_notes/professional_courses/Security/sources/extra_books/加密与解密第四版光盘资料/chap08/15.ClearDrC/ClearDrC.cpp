/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// ClearDrC.cpp : Defines the entry point for the console application.
// Author:achillis
// 本程序为演示在异常处理程序中进行反调试操作，清空dr寄存器以使硬件断点失效

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	PCONTEXT pContext = NULL ;
	__try
	{
		*(ULONG*)NULL = 5 ;
	}
	__except(pContext = (GetExceptionInformation())->ContextRecord,EXCEPTION_EXECUTE_HANDLER)
	{
		printf("Now Clear Dr Registers.\n");
		pContext->Dr0 = 0 ;
		pContext->Dr1 = 0 ;
		pContext->Dr2 = 0 ;
		pContext->Dr3 = 0 ;
		pContext->Dr7 = 341 ;
	}
	return 0;
}

