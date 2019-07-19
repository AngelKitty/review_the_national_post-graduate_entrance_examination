/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 头文件
#include "stdafx.h"
#include <Windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
#pragma comment(linker, "/EXPORT:LpkInitialize=lpkOrg.LpkInitialize,@1")
#pragma comment(linker, "/EXPORT:LpkTabbedTextOut=lpkOrg.LpkTabbedTextOut,@2")
#pragma comment(linker, "/EXPORT:LpkDllInitialize=lpkOrg.LpkDllInitialize,@3")
#pragma comment(linker, "/EXPORT:LpkDrawTextEx=lpkOrg.LpkDrawTextEx,@4")
#pragma comment(linker, "/EXPORT:LpkEditControl=lpkOrg.LpkEditControl,@5")
#pragma comment(linker, "/EXPORT:LpkExtTextOut=lpkOrg.LpkExtTextOut,@6")
#pragma comment(linker, "/EXPORT:LpkGetCharacterPlacement=lpkOrg.LpkGetCharacterPlacement,@7")
#pragma comment(linker, "/EXPORT:LpkGetTextExtentExPoint=lpkOrg.LpkGetTextExtentExPoint,@8")
#pragma comment(linker, "/EXPORT:LpkPSMTextOut=lpkOrg.LpkPSMTextOut,@9")
#pragma comment(linker, "/EXPORT:LpkUseGDIWidthCache=lpkOrg.LpkUseGDIWidthCache,@10")
#pragma comment(linker, "/EXPORT:ftsWordBreak=lpkOrg.ftsWordBreak,@11")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


DWORD WINAPI ThreadWorking(LPVOID lpParameters);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 入口函数
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL,0,ThreadWorking,NULL,0,NULL);
		DisableThreadLibraryCalls(hModule);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}

	return TRUE;
}

DWORD WINAPI ThreadWorking(LPVOID lpParameters)
{
	Sleep(500);
	MessageBox(NULL,"Fake Lpk loaded!","Notice",MB_OK);
	OutputDebugString("Lpk.dll is working.\n");
	return 0 ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
