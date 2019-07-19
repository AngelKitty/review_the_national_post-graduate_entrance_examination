/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/
// Showx86SEH.cpp : Defines the entry point for the console application.
// Author:achillis
// 编译环境：VC6/VS2010（或其它版本VS均可）
// 本程序用于演示VC编译器对异常的封装处理，包含了经常遇到的各种情况
// 请读者自行调试本程序，或使用IDA Pro分析本程序的编译后的代码

#include "stdafx.h"
#include <windows.h>

DWORD ShowFinalNormal();
DWORD ShowFinalDirectRetn();
DWORD ShowFinalWithLeave();
DWORD ShowFinalNotCatchExp();
VOID OneException();
VOID OneUnHandleException();
VOID TwoException();
VOID ThreeException();
VOID ShowCPPException();
VOID CatchSubFunException();


int main(int argc, char* argv[])
{
	
	OneException();
	printf("================One Exception End====================\n");
	TwoException();
	printf("================Two Exception End====================\n");
	ThreeException();
	printf("===============Three Exception End===================\n");
	CatchSubFunException();
	printf("================Catch Subfun Exception End===========\n");
	ShowFinalNormal();
	printf("==================FinalNormal End====================\n");
	ShowFinalDirectRetn();
	printf("=================FinalDirectRetn End=================\n");
	ShowFinalWithLeave();
	printf("=================FinalWithLeave End==================\n");
	ShowFinalNotCatchExp();
	printf("=================FinalNotCatchExp End================\n");
	return 0;
}


//单个try/except组合,处理异常
VOID OneException()
{
	int *pValue = NULL ;
	__try
	{
		printf("In Try.\n");
		*pValue = 0x55555555;
	}
	__except(printf("In Filter\n"),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("In handler.\n");
	}
}

//单个try/except组合,不处理异常
VOID OneUnHandleException()
{
	int *pValue = NULL ;
	__try
	{
		printf("[SubFun] In Try.\n");
		*pValue = 0x55555555;
	}
	__except(printf("[SubFun] In Filter\n"),EXCEPTION_CONTINUE_SEARCH)
	{
		printf("[SubFun] In handler.\n");
	}
}

//捕获子函数的异常
VOID CatchSubFunException()
{
	__try
	{
		printf("In Try.\n");
		OneUnHandleException();
	}
	__except(printf("In Filter\n"),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("In handler.\n");
	}
}

//两组嵌套异常
VOID TwoException()
{
	int *pValue = NULL ;
	__try
	{
		printf("In Try0.\n");
		__try
		{
			printf("In Try1.\n");
			*pValue = 0x55555555;
		}
		__except(printf("In Filter1\n"),EXCEPTION_CONTINUE_SEARCH)
		{
			printf("In Handler1.\n");
		}
	}
	__except(printf("In Filter0\n"),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("In Handler0.\n");
	}
	
	printf("After All Trys.\n");
}

//多组混合组合
VOID ThreeException()
{
	int *pValue = NULL ;
	__try
	{
		printf("In Try0.\n");
		__try
		{
			printf("In Try1.\n");
			__try
			{
				printf("In Try2.\n");
				*pValue = 0x55555555;
			}
			__except(printf("In Filter2\n"),EXCEPTION_CONTINUE_SEARCH)
			{
				printf("In Handler2.\n");
			}
		}
		__except(printf("In Filter1\n"),EXCEPTION_CONTINUE_SEARCH)
		{
			printf("In Handler1.\n");
		}
	}
	__except(printf("In Filter0\n"),EXCEPTION_EXECUTE_HANDLER)
	{
		printf("In Handler0.\n");
	}
	
	printf("After All Trys.\n");
}

//普通的try/finally
DWORD ShowFinalNormal()
{
	DWORD dwValue = 0x11111111 ;
	__try
	{
		printf("In Try.\n");
		dwValue = GetTickCount();
	}
	__finally
	{
		printf("In Finally.\n");
	}

	return dwValue;
}

//在try中直接return
DWORD ShowFinalDirectRetn()
{
	DWORD dwValue = 0x11111111 ;
	__try
	{
		printf("In Try.\n");
		dwValue = GetTickCount();
		if (dwValue % 2 == 0)
		{
			return dwValue;
		}
	}
	__finally
	{
		printf("In Finally.\n");
	}
	
	return dwValue;
}

//使用leave离开try域
DWORD ShowFinalWithLeave()
{
	DWORD dwValue = 0x11111111 ;
	__try
	{
		printf("In Try.\n");
		dwValue = GetTickCount();
		if (dwValue % 2 == 0)
		{
			printf("Begin to Leave try.\n");
			__leave;
		}
	}
	__finally
	{
		printf("In Finally.\n");
	}
	
	return dwValue;
}

//在try中引发异常,但不处理，这将导致展开时finally块被执行
DWORD ShowFinalNotCatchExp()
{
	int *pValue = NULL ;
	__try
	{
		printf("In Try.\n");
		*pValue = 5 ;
	}
	__finally
	{
		printf("In Finally.\n");
	}
	
	return 0;
}
