/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 3.4 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: hash.c
author		: failwest  
date		: 2006.10.20
description	: used to calculate digest of function name for general shellcode   
Noticed		:  
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/
#include <stdio.h>
#include <windows.h>
DWORD GetHash(char *fun_name)
{
	DWORD digest=0;
	while(*fun_name)
	{
		digest=((digest<<25)|(digest>>7));
		digest+= *fun_name ;
		fun_name++;
	}
	return digest;
}
main()
{
	DWORD hash;
	hash= GetHash("MessageBoxA");
	printf("result of hash is %.8x\n",hash);
}