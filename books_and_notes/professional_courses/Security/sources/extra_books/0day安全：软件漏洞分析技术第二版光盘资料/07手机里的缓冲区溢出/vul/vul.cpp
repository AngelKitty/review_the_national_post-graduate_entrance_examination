// vul.cpp : Defines the entry point for the console application.
//

#include"stdafx.h"
#include<windows.h>
#include<commctrl.h>
int display(void)
{
	FILE * fp;
	char buf[16];
	if(!(fp=fopen("\\exploit.txt","r")))
		return 0;
	fread(buf,1,100,fp);
	printf("file: %s",buf);
	return 1;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int flag=0;
	flag=display();
	return 0;
}