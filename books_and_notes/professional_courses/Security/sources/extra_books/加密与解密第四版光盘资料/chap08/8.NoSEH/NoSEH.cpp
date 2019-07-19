// NoSEH.cpp : Defines the entry point for the console application.
//
/*-----------------------------------------------------------------------
第8章  Windows下的异常处理
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/
#include "stdafx.h"

#pragma comment(linker,"/Entry:main")
#pragma comment(linker,"/subsystem:windows")

__declspec(naked) void main(void)
{
	__asm
	{
		;mov dword ptr fs:[0],-1
		xor eax,eax
		mov [eax],5 //向0地址写入数据，引发内存访问异常
		retn
	}
}

