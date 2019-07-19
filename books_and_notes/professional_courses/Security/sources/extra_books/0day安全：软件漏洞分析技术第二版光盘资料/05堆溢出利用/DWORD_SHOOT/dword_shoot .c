/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 6.3 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: dword_shoot.c
author		: failwest  
date		: 2007.04.04
description	: demo show of dword shooting in heap overrun
Noticed		:	1 only run on windows 2000
				2 complied with VC 6.0
				3 build into release version
				4 used for run time debugging
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

#include <windows.h>
main()
{
	HLOCAL h1, h2,h3,h4,h5,h6;
	HANDLE hp;
	hp = HeapCreate(0,0x1000,0x10000);
	h1 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);
	h2 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);
	h3 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);
	h4 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);
	h5 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);
	h6 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);

	_asm int 3	//used to break the process
	//free the odd blocks to prevent coalesing
	HeapFree(hp,0,h1); 
	HeapFree(hp,0,h3); 
	HeapFree(hp,0,h5); //now freelist[2] got 3 entries
	
	//will allocate from freelist[2] which means unlink the last entry (h5)
	h1 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8); 
		
	return 0;
}
