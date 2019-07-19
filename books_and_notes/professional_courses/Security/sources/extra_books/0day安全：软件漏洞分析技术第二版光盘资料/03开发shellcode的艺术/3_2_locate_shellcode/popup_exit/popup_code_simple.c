/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 3.2 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: popup_code_simple.c
author		: failwest  
date		: 2006.10.10
description	: popup a message box show "failwest" and exit cleanly
				used to generate PE file and extract binary code 
Noticed		: the address of API should reset before compiling
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

#include <windows.h>
int main()
{	
	HINSTANCE LibHandle;
	char dllbuf[11] = "user32.dll";
	LibHandle = LoadLibrary(dllbuf);
	_asm{
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				sub sp,0x440
				xor ebx,ebx
				push ebx // cut string
				push 0x74736577
				push 0x6C696166//push failwest
			
				mov eax,esp //load address of failwest
				push ebx	
				push eax
				push eax
				push ebx
				
				mov  eax, 0x7E45058A//(0x77D804EA) address should be reset in different OS
				call eax  //call MessageboxA
							
				push ebx
				mov eax,0x7C81CDDA //(0x7C81CDDA) address should be reset in different OS
				call eax //call exit(0)
				
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
				nop
	}
}
