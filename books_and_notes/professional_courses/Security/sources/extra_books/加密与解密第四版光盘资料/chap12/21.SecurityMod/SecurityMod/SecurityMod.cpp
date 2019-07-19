/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// SecurityMod.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SecurityMod.h"
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<DWORD> ThreadVector;

extern "C" SECURITYMOD_API BOOL RegisterThread(DWORD dwThreadId);
VOID UnRegisterThread(DWORD dwThreadId);
BOOL IsTrustedThread(DWORD dwThreadId);

#define MAX_THREAD_CNT (1024)
DWORD g_ThreadIs[MAX_THREAD_CNT]={0};
DWORD g_ThreadCnt = 0 ;
BOOL  g_EnableProtect = FALSE;
CRITICAL_SECTION g_cs;
ThreadVector g_ThreadVector;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	DWORD dwCurThreadId = GetCurrentThreadId();
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				InitializeCriticalSection(&g_cs);
				printf("[SecurityMod] Loading.\n");
				break;
			}
		case DLL_THREAD_ATTACH:
			{
				if (!IsTrustedThread(dwCurThreadId))
				{
					printf("ThreadId = %d . 非法线程，退出.\n",dwCurThreadId);
					ExitThread(0);
				}
				else
				{
					printf("ThreadId = %d . 信任线程，放行。\n",dwCurThreadId);
				}
				
				break;
			}
		case DLL_THREAD_DETACH:
			{
				if (IsTrustedThread(dwCurThreadId))
				{
					printf("受信任线程 %d 正在退出.\n",dwCurThreadId);
					UnRegisterThread(dwCurThreadId);
				}
				break;
			}
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

extern "C" SECURITYMOD_API BOOL RegisterThread(DWORD dwThreadId)
{
	BOOL bResult = FALSE ;
	EnterCriticalSection(&g_cs);
	g_ThreadVector.push_back(dwThreadId);
	g_ThreadCnt++;
	LeaveCriticalSection(&g_cs);
	return TRUE;
}

VOID UnRegisterThread(DWORD dwThreadId)
{
	ThreadVector::iterator theIterator;
	EnterCriticalSection(&g_cs);
	theIterator = find(g_ThreadVector.begin(),g_ThreadVector.end(),dwThreadId);
	if (theIterator != g_ThreadVector.end())
	{
		//Found
		g_ThreadVector.erase(theIterator);
		g_ThreadCnt--;
	}
	LeaveCriticalSection(&g_cs);
}

BOOL IsTrustedThread(DWORD dwThreadId)
{
	BOOL bResult = FALSE ;
	DWORD i = 0 ;

	if (g_ThreadVector.size() == 0)
	{
		//尚未注册时，对所有线程放行
		return TRUE;
	}

	ThreadVector::iterator theIterator;
	EnterCriticalSection(&g_cs);
	theIterator = find(g_ThreadVector.begin(),g_ThreadVector.end(),dwThreadId);
	if (theIterator != g_ThreadVector.end())
	{
		//Found
		bResult = TRUE;
	}
	LeaveCriticalSection(&g_cs);
	return bResult;
}
