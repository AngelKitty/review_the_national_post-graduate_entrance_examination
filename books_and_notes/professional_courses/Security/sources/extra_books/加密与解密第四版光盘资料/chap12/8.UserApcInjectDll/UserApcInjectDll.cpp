/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// UserApcInjectDll.cpp : Defines the entry point for the console application.
// Tested On:WinXP SP3/Win7 x86/Win7 x64

#include "stdafx.h"
#include <windows.h>
#include <TLHELP32.H>

DWORD ProcesstoPid(char *Processname) ;
BOOL InjectModuleToProcessById(DWORD dwPid, char *szDllFullPath);

#ifdef _WIN64
char g_szDllPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\MsgDll64.dll" ;
char g_szProcName[MAX_PATH] = "HostProc64.exe";
#else
char g_szDllPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\MsgDll.dll" ;
char g_szProcName[MAX_PATH] = "HostProc.exe";
#endif

int main(int argc, char* argv[])
{
	DWORD dwPid = ProcesstoPid(g_szProcName);
	if (dwPid == 0)
	{
		printf("未找到目标进程!\n");
		return 0;
	}
	printf("Target Pid = %d\n",dwPid);
	BOOL bResult = InjectModuleToProcessById(dwPid,g_szDllPath);
	printf("Result = %d\n",bResult);
	return 0;
}

BOOL InjectModuleToProcessById(DWORD dwPid, char *szDllFullPath)  
{  
    SIZE_T   stSize = 0 ;  
    BOOL    bStatus = FALSE ;  
    LPVOID  lpData = NULL ;  
    SIZE_T  uLen = lstrlen(szDllFullPath) + 1;  
	
    // 打开目标进程  
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid) ;  
    if (hProcess)  
    {  
        // 分配空间  
        lpData = VirtualAllocEx(hProcess, NULL, uLen, MEM_COMMIT, PAGE_READWRITE);  
        DWORD dwErr = GetLastError();  
        if (lpData)  
        {  
            // 写入需要注入的模块路径全名  
            bStatus = WriteProcessMemory(hProcess, lpData, szDllFullPath, uLen, &stSize) ;  
        }  
		
        CloseHandle(hProcess) ;  
    }  
	
	//以上操作与创建远程线程的准备工作相同

    if (bStatus == FALSE)  
        return FALSE ;  
	
    // 创建线程快照  
    THREADENTRY32 te32 = {sizeof(THREADENTRY32)} ;  
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0) ;  
    if (hThreadSnap == INVALID_HANDLE_VALUE)   
        return FALSE ;   
	
    bStatus = FALSE ;  
    // 枚举所有线程  
    if (Thread32First(hThreadSnap, &te32))  
    {  
        do{  
            // 判断是否目标进程中的线程  
            if (te32.th32OwnerProcessID == dwPid)  
            {  
                // 打开线程  
                HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID) ;  
                if (hThread)  
                {  
                    // 向指定线程添加APC  
                    DWORD dwRet = QueueUserAPC ((PAPCFUNC)LoadLibraryA, hThread, (ULONG_PTR)lpData) ;  
                    if (dwRet > 0)  
                        bStatus = TRUE ;  
                    CloseHandle (hThread) ;  
                }  
            }   
			
        }while (Thread32Next ( hThreadSnap, &te32 )) ;  
    }  
	
    CloseHandle (hThreadSnap) ;  
    return bStatus;  
}

DWORD ProcesstoPid(char *Processname) //查找指定进程的PID(Process ID)
{
	HANDLE hProcessSnap=NULL;
	DWORD ProcessId=0;
	PROCESSENTRY32 pe32={0};
	hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0); //打开进程快照
	if(hProcessSnap==(HANDLE)-1)
	{
		printf("\nCreateToolhelp32Snapshot() Error: %d",GetLastError());
		return 0;
	}
	pe32.dwSize=sizeof(PROCESSENTRY32);
	if(Process32First(hProcessSnap,&pe32)) //开始枚举进程
	{
		do
		{
			if(!stricmp(Processname,pe32.szExeFile)) //判断是否和提供的进程名相等，是，返回进程的ID
			{
				ProcessId=pe32.th32ProcessID;
				break;
			}
		}
		while(Process32Next(hProcessSnap,&pe32)); //继续枚举进程
	}
	else
	{
		printf("\nProcess32First() Error: %d",GetLastError());
		return 0;
	}
	CloseHandle(hProcessSnap); //关闭系统进程快照的句柄
	return ProcessId;
}