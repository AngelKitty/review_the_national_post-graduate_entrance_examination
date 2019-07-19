/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// HookPromt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

void PrintChar(char *pch);

int main(int argc, char* argv[])
{
	char ch = 'A';
	char ch2 = 'B';
	char *pChar ;
	
	//正常调用
	pChar = &ch;
	PrintChar(pChar);
	//Address Hook,修改传递的地址
	pChar = &ch2;
	PrintChar(&ch2);
	//恢复原值
	pChar = &ch;
	//Inline  Hook,地址不变,修改传递的内容
	*pChar = 'B';
	PrintChar(pChar);
	return 0 ;

}

void PrintChar(char *pch)
{
	printf("Address = 0x%x Char = %c\n",pch,*pch);
}