/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 6.2 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: heap_debug.c
author		: failwest  
date		: 2007.04.04
description	: demo show of how heap works
Noticed		:	1 only run on windows 2000
				2 complied with VC 6.0
				3 build into release version
				4 only used for run time debugging
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

#include <windows.h>
main()
{
	HLOCAL h1,h2,h3,h4,h5,h6;
	HANDLE hp;
	hp = HeapCreate(0,0x1000,0x10000);
	__asm int 3

	h1 = HeapAlloc(hp,HEAP_ZERO_MEMORY,3);
	h2 = HeapAlloc(hp,HEAP_ZERO_MEMORY,5);
	h3 = HeapAlloc(hp,HEAP_ZERO_MEMORY,6);
	h4 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);
	h5 = HeapAlloc(hp,HEAP_ZERO_MEMORY,19);
	h6 = HeapAlloc(hp,HEAP_ZERO_MEMORY,24);
	
	//free block and prevent coaleses
	HeapFree(hp,0,h1); //free to freelist[2] 
	HeapFree(hp,0,h3); //free to freelist[2] 
	HeapFree(hp,0,h5); //free to freelist[4]
	
	HeapFree(hp,0,h4); //coalese h3,h4,h5,link the large block to freelist[8]

	
	return 0;
}
