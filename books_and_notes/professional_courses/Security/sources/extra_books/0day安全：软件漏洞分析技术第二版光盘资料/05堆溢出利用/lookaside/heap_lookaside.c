/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 5 in book "Lookaside Using"
 
file name	: heap_lookaside.c
author		: failwest  
date		: 2010.09.04
description	: demo show of how heap works
Noticed		:	1 only run on windows 2000
				2 complied with VC 6.0
				3 build into release version
				4 only used for run time debugging
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

#include <stdio.h>
#include <windows.h>
void main()
{
	HLOCAL h1,h2,h3,h4;
	HANDLE hp;
	hp = HeapCreate(0,0,0);
	__asm int 3
	h1 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);
	h2 = HeapAlloc(hp,HEAP_ZERO_MEMORY,8);
	h3 = HeapAlloc(hp,HEAP_ZERO_MEMORY,16);
	h4 = HeapAlloc(hp,HEAP_ZERO_MEMORY,24);
	HeapFree(hp,0,h1);
	HeapFree(hp,0,h2);
	HeapFree(hp,0,h3);
	HeapFree(hp,0,h4);
	h2 = HeapAlloc(hp,HEAP_ZERO_MEMORY,16);
	HeapFree(hp,0,h2);
}


