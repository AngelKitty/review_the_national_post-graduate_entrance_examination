/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 3.2 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: search_opcode.cpp
author		: failwest  
date		: 2006.10.7
description	: used to search addresses of jump instruction in memory
Noticed		: code can be changed easily to search different instruction in different file  
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

//FF E0		JMP EAX
//FF E1		JMP ECX
//FF E2		JMP EDX
//FF E3		JMP EBX
//FF E4		JMP ESP
//FF E5		JMP EBP
//FF E6		JMP ESI
//FF E7		JMP EDI

//FF D0		CALL EAX
//FF D1		CALL ECX
//FF D2		CALL EDX
//FF D3		CALL EBX
//FF D4		CALL ESP
//FF D5		CALL EBP
//FF D6		CALL ESI
//FF D7		CALL EDI


//#define DLL_NAME "mfc42.dll"

#include <windows.h>
#include <stdio.h>
#define DLL_NAME "user32.dll"
main()
{
	BYTE* ptr;
	int position,address;
	HINSTANCE handle;
	BOOL done_flag = FALSE;

	handle=LoadLibrary(DLL_NAME);

	if(!handle)
	{
		printf(" load dll erro !");
		exit(0);
	}

	ptr = (BYTE*)handle;
	
	for(position = 0; !done_flag; position++)
	{
		try
		{
			if(ptr[position] == 0xFF && ptr[position+1] == 0xE4)
			{
				//0xFFE4 is the opcode of jmp esp
				int address = (int)ptr + position;
				printf("OPCODE found at 0x%x\n",address);
			}
		}
		catch(...)
		{
			int address = (int)ptr + position;
			printf("END OF 0x%x\n", address);
			done_flag = true;
		}
	}
}