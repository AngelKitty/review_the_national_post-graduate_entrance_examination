/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// InjectDll.cpp : Defines the entry point for the console application.
// 向一个已经运行中的进程以远程线程的方式注入dll

#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h>

BOOL WINAPI InjectDllToProcess(DWORD dwTargetPid ,LPCTSTR DllPath );
DWORD ProcesstoPid(char *Processname);
BOOL EnableDebugPrivilege();

int main(int argc, char* argv[])
{
#ifdef _WIN64
	char szProcName[MAX_PATH] = "HostProc64.exe";
	char szDllPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\MsgDll64.dll";
#else
	char szProcName[MAX_PATH] = "HostProc.exe";
	char szDllPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\MsgDll.dll";
#endif
	
	DWORD dwPid = ProcesstoPid(szProcName);
	EnableDebugPrivilege();
	InjectDllToProcess(dwPid,szDllPath);
	return 0;
}

//注入主函数
BOOL WINAPI InjectDllToProcess(DWORD dwTargetPid ,LPCTSTR DllPath )
{
    HANDLE hProc = NULL;
	hProc=OpenProcess(PROCESS_ALL_ACCESS,//Win7下要求的权限较高
		FALSE,
		dwTargetPid
		);

    if(hProc == NULL)
    {
        printf("[-] OpenProcess Failed.\n");
        return FALSE;
    }
	
    LPTSTR psLibFileRemote = NULL;
	
    //使用VirtualAllocEx函数在远程进程的内存地址空间分配DLL文件名缓冲
    psLibFileRemote=(LPTSTR)VirtualAllocEx(hProc, NULL, lstrlen(DllPath)+1,
		MEM_COMMIT, PAGE_READWRITE);
	
    if(psLibFileRemote == NULL)
    {
        printf("[-] VirtualAllocEx Failed.\n");
        return FALSE;
    }
	
    //使用WriteProcessMemory函数将DLL的路径名复制到远程的内存空间
    if(WriteProcessMemory(hProc, psLibFileRemote, (void *)DllPath, lstrlen(DllPath)+1, NULL) == 0)
    {
        printf("[-] WriteProcessMemory Failed.\n");
        return FALSE;
    }
	
    //计算LoadLibraryA的入口地址
    PTHREAD_START_ROUTINE pfnStartAddr=(PTHREAD_START_ROUTINE)
        GetProcAddress(GetModuleHandle("Kernel32"),"LoadLibraryA");
	
    if(pfnStartAddr == NULL)
    {
        printf("[-] GetProcAddress Failed.\n");
        return FALSE;
    }

    //pfnStartAddr地址就是LoadLibraryA的入口地址
    HANDLE hThread = CreateRemoteThread(hProc,
        NULL,
        0,
        pfnStartAddr,
        psLibFileRemote,
        0,
        NULL);
	
    if(hThread == NULL)
    {
        printf("[-] CreateRemoteThread Failed. ErrCode = %d\n",GetLastError());
        return FALSE;
    }

	printf("[*]Inject Succesfull.\n");
    return TRUE;
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

BOOL EnableDebugPrivilege() //本函数用于提升权限，提升到SE_DEBUG_NAME
{ 
	TOKEN_PRIVILEGES tkp; 
	HANDLE hToken; 
	if (!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))     //打开当前进程失败 
		return FALSE; 
	LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid); //查看当前权限
	tkp.PrivilegeCount = 1; 
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); //调整权限，如上设置
	return TRUE; 
}