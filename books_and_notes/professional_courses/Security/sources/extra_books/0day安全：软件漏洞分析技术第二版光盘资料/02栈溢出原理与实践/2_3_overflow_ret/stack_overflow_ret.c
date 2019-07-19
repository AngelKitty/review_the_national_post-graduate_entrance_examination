/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 2.3 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: stack_overflow_ret.c
author		: failwest  
date		: 2006.9.30
description	: demo show to redirect program execute flow via over run return address
				in stack. specify the exactly fake return address in password.txt file 
				to bypass the authentication  
Noticed		: should be complied with VC6.0 and build into debug version  
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/
#include <stdio.h>
#define PASSWORD "1234567"
int verify_password (char *password)
{
	int authenticated;
	char buffer[8];
	authenticated=strcmp(password,PASSWORD);
	strcpy(buffer,password);//over flowed here!	
	return authenticated;
}
main()
{
	int valid_flag=0;
	char password[1024];
	FILE * fp;
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