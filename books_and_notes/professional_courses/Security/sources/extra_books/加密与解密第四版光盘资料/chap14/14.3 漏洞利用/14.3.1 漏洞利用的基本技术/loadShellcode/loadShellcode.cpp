/*-----------------------------------------------------------------------
第14章  漏洞分析技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// loadShellcode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	HANDLE fp;
	unsigned char* fBuffer;
	DWORD fSize,dwSize;
	fp = CreateFile("shellcode_bin.bin",GENERIC_READ,FILE_SHARE_READ,NULL,
		OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	fSize = GetFileSize(fp,0);
	fBuffer =  (unsigned char *)VirtualAlloc(NULL, fSize,MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	ReadFile(fp,fBuffer,fSize,&dwSize,0);
	CloseHandle(fp);
	
	_asm
	{
		pushad
			mov eax, fBuffer
			call eax
			popad
	}
	printf("Hello World!\n");
	return 0;
}

