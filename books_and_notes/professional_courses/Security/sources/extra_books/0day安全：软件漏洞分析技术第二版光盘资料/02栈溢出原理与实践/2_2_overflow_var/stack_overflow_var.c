/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 2.2 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: stack_overflow_var.c
author		: failwest  
date		: 2006.9.20
description	: demo show nearby var overrun in stack
			  input 8 letters to bypass authentication  
Noticed		: complied with VC6.0 and build into begug version
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/
#include <stdio.h>

#define PASSWORD "1234567"

int verify_password (char *password)
{
	int authenticated;
	char buffer[8];// add local buff
	authenticated=strcmp(password,PASSWORD);
	strcpy(buffer,password);//over flowed here!	
	return authenticated;
}


main()
{
	int valid_flag=0;
	char password[1024];
	while(1)
	{
		printf("please input password:       ");
		
		scanf("%s",password);
		
		valid_flag = verify_password(password);
		
		if(valid_flag)
		{
			printf("incorrect password!\n\n");
		}
		else
		{
			printf("Congratulation! You have passed the verification!\n");
			break;
		}
	}
}
