//===========================================================================
/*
* 第10章  VT技术
*《加密与解密（第四版）》
*(c)  看雪学院 www.kanxue.com 2000-2018
*
 * Split.c	:TLB内存分割测试程序
 * Author	:Joen
 * QQ		:51753931
 * E-mail	:Joen@JoenChen.com
 * Website	:http://www.joenchen.com
 * 将一个PE Section代码和数据映射到不同的内容上!
 */
//===========================================================================
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <assert.h>
#include <iphlpapi.h>

#pragma data_seg("NewSeon")

int PrintValue = 0x9090c390;

#pragma data_seg()

#pragma comment(linker, "/SECTION:NewSeon,ERW")

int __cdecl main( int argc, char* argv[] )
{
	printf( "value %x\n", PrintValue );
	
	__asm
	{
		lea 	eax, PrintValue
		call 	eax
	}
	
	system("PAUSE");
	
	return 0;
}