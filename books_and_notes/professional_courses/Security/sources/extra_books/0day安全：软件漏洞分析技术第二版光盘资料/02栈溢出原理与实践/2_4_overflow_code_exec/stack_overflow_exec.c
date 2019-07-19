/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 2.4 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: stack_overflow_exec.c
author		: failwest  
date		: 2006.10.1
description	: demo show how to redirect EIP to executed extra binary code in buffer
Noticed		: should be complied with VC6.0 and build into debug version
				the address of MessageboxA and the start of machine code in buffer
				have to be make sure in file "password.txt" via runtime debugging
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/
#include <stdio.h>
#include <windows.h>
#define PASSWORD "1234567"
int verify_password (char *password)
{
	int authenticated;
	char buffer[44];
	authenticated=strcmp(password,PASSWORD);
	strcpy(buffer,password);//over flowed here!	
	return authenticated;
}
main()
{
	int valid_flag=0;
	char password[1024];
	FILE * fp;
	LoadLibrary("user32.dll");//prepare for messagebox
	if(!(fp=fopen("password.txt","rw+")))
	{
		exit(0);
	}
	fscanf(fp,"%s",password);
	valid_flag = verify_password(password);
	if(valid_flag)
	{
		printf("incorrect password!\n");
	}
	else
	{
		printf("Congratulation! You have passed the verification!\n");
	}
	fclose(fp);
}