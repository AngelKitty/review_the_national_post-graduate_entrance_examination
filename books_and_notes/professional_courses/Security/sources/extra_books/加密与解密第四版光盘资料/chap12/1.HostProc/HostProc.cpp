/*-----------------------------------------------------------------------
*第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// HostProc.cpp : Defines the entry point for the console application.
// 一个用于演示进程注入的宿主

#include "stdafx.h"
#include <windows.h>

//固定基址，尤其是编译为x64时，在编译参数中把随机基址设置为NO,方便调试
#pragma comment(linker,"/Base:0x20000000")

DWORD WINAPI ThreadProc(LPVOID lpParameters);

int main(int argc, char* argv[])
{
	printf("[Process]PID = %d, Host Process Working!\n",GetCurrentProcessId());
	//LoadLibrary("F:\\Program2016\\DllInjection\\SecurityMod.dll");
	CreateThread(NULL,0,ThreadProc,NULL,0,NULL);
	while (TRUE)
	{
		Sleep(20000);
	}
	
	return 0;
}

//For APC Inject
DWORD WINAPI ThreadProc(LPVOID lpParameters)
{
	while (TRUE)
	{
		SleepEx(3000,TRUE);
	}
	printf("Waked up from Sleep.\n");
	return 0 ;
}
