/*-----------------------------------------------------------------------
第14章  漏洞分析技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// getKernel32.cpp : Defines the entry point for the console application.
//实验环境：
//windows xp sp3 + VC6.0 
//编译选项：Debug（默认配置）


#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	DWORD hKernel32 = 0;
	__asm
	{
		//assume	 	fs:nothing
		mov   		eax, fs:[30h]
		test  		eax, eax
		js  		os_9x
os_nt:  
		mov  		eax, dword ptr[eax+0ch]
		mov  		esi, dword ptr[eax+1ch]
		lodsd  
		mov   		eax, dword ptr[eax+8]
		jmp  		k_finished
os_9x:
		mov   		eax, dword ptr[eax+34h]
		mov  		eax, dword ptr[eax+7ch]
		mov  		eax, dword ptr[eax+3ch]
k_finished:
		mov  		hKernel32, eax  ;获取kernel32地址 

	}
	printf("hKernel32 = %x\n",hKernel32);
	return 0;
}

