/*-----------------------------------------------------------------------
第14章  漏洞分析技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// stack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

void testFunc(char *Buf)
{
	char testBuf[8];
	memcpy(testBuf,Buf,32);
	return;
}

int main(int argc, char* argv[])
{
	char Buf[64]={0};
	memset(Buf,0x41,64);
	testFunc(Buf);
	return 0;
}

