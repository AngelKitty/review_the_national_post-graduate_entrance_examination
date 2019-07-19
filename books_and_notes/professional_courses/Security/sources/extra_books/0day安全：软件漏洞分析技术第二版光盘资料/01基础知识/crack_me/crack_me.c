/*****************************************************************************
      To be the apostrophe which changed "Impossible" into "I'm possible"!
		
POC code of chapter 3.7 in book "Vulnerability Exploit and Analysis Technique"
 
file name	: crack_me.c
author		: failwest  
date		: 2006.9.20
description	: used as a simple demo to show how to crack a PE file   
Noticed		: should be complied with VC6.0 and build into release version  
version		: 1.0
E-mail		: failwest@gmail.com
		
	Only for educational purposes    enjoy the fun from exploiting :)
******************************************************************************/

#include <stdio.h>
#define PASSWORD "1234567"
int verify_password (char *password)
{
	int authenticated;
	authenticated=strcmp(password,PASSWORD);
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