/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// SetContextInject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <TLHELP32.H>
#include <Psapi.h>

#pragma comment(lib,"psapi.lib")

typedef struct _INJECT_DATA 
{
	BYTE ShellCode[0x30];
	ULONG_PTR AddrofLoadLibraryA;
	PBYTE lpDllPath;
	ULONG_PTR OriginalEIP;
	char szDllPath[MAX_PATH];
}INJECT_DATA;

#ifdef _WIN64
EXTERN_C VOID ShellCodeFun64(VOID);
#else
VOID ShellCodeFun(VOID);
#endif

ULONG_PTR GetModuleHandleInProcess( DWORD processID , char *szModuleName );
DWORD ProcesstoPid(char *Processname);
BOOL InjectModuleToProcessBySetContext(DWORD dwPid, char *szDllFullPath);
VOID PrepareShellCode(BYTE *pShellCode);

int main(int argc, char* argv[])
{
#ifdef _WIN64
	char szDllPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\MsgDll64.dll" ;
	DWORD dwPid = ProcesstoPid("HostProc64.exe");
#else
	char szDllPath[MAX_PATH] = "F:\\Program2016\\DllInjection\\MsgDll.dll" ;
	DWORD dwPid = ProcesstoPid("HostProc.exe");
#endif
	if (dwPid == 0)
	{
		printf("[-] Target Process Not Found!\n");
		return 0;
	}
	printf("[*] Target Process Pid = %d\n",dwPid);
	BOOL bResult = InjectModuleToProcessBySetContext(dwPid,szDllPath);
	printf("[*] Result = %d\n",bResult);
	return 0;
}




#ifndef _WIN64
__declspec (naked)
VOID ShellCodeFun(VOID)
{
	__asm
	{
		push eax //占位,一会儿用做跳转地址
		pushad   //大小0x20
		pushfd   //大小0x04
		call L001
L001:
		pop ebx
		sub ebx,8
		push dword ptr ds:[ebx+0x34] //szDllPath
		call dword ptr ds:[ebx+0x30] //LoadLibraryA
		mov eax,dword ptr ds:[ebx+0x38] //OriginalEIP
		xchg eax,[esp+0x24] //将原来的EIP交换到栈上
		popfd
		popad
		retn //jmp to OriginalEIP
		nop
		nop
		nop
		nop
		nop
	}
}
#endif


VOID PrepareShellCode(BYTE *pOutShellCode)
{
#ifdef _WIN64
	//x64不能使用__asm了，只能直接写Shellcode或单独写在asm文件中
	/*
	BYTE ShellCode64[]=
		"\x50"					//push    rax
		"\x53"					//push    rbx
		"\x9c"					//pushfq
		"\xe8\x00\x00\x00\x00"  //call  next
		"\x5b"					//pop     rbx
		"\x66\x83\xe3\x00"      //and     bx,0
		"\x48\x8b\x4b\x38"      //mov     rcx,qword ptr [rbx+38h] ; lpDllPath
		"\xff\x53\x30"          //call    qword ptr [rbx+30h]		;AddrofLoadLibraryA
		"\x48\x8b\x43\x40"      //mov     rax,qword ptr [rbx+40h]	;OriginalEIP
		"\x48\x87\x44\x24\x10"  //xchg    rax,qword ptr [rsp+10h]	;saved retnaddr
		"\x9d"					//popfq
		"\x5b"					//pop     rbx
		"\xc3"					//ret
		"\x90"					//nop
	;

	memcpy(pOutShellCode,ShellCode64,33);
	*/
	BYTE *pShellcodeStart = (BYTE*)ShellCodeFun64;
#else
	BYTE *pShellcodeStart = (BYTE*)ShellCodeFun;
#endif

	BYTE *pShellcodeEnd = 0 ;
	SIZE_T ShellCodeSize = 0 ;
	if (pShellcodeStart[0] == 0xE9)
	{
		//Debug模式下，函数开头是一个跳转指令，这里取它的真正地址
		pShellcodeStart = pShellcodeStart + *(ULONG*)(pShellcodeStart +1 ) + 5;
	}
	
	//搜索Shellcode结束标志
	pShellcodeEnd = pShellcodeStart;
	while (memcmp(pShellcodeEnd,"\x90\x90\x90\x90\x90",5) != 0)
	{
		pShellcodeEnd++;
	}
	
	ShellCodeSize = pShellcodeEnd - pShellcodeStart;
	printf("[*] Shellcode Len = %d\n",ShellCodeSize);
	memcpy(pOutShellCode,pShellcodeStart,ShellCodeSize);


}
BOOL InjectModuleToProcessBySetContext(DWORD dwPid, char *szDllFullPath)
{
	SIZE_T   dwRet = 0 ;  
    BOOL    bStatus = FALSE ;  
    PBYTE   lpData = NULL ;  
    SIZE_T  uLen = 0x1000;
	INJECT_DATA Data;
	HANDLE hProcess,hThread;
	DWORD i = 0 ;
	

	//1.获取目标进程中LoadLibraryA的地址
	//之所以这么获取，是考虑了ASLR的影响，此时目标进程中kernel32.dll的加载位置不一定与本进程相同
	ULONG_PTR uKernelBaseInTargetProc = GetModuleHandleInProcess(dwPid,"kernel32.dll");
	ULONG_PTR uKernelBaseInCurProc = (ULONG_PTR)GetModuleHandle("kernel32.dll");
	ULONG_PTR uLoadLibraryAddrInCurProc =  (ULONG_PTR)GetProcAddress((HMODULE)uKernelBaseInTargetProc,"LoadLibraryA");
	ULONG_PTR uLoadLibraryAddrInTargetProc = uLoadLibraryAddrInCurProc -  uKernelBaseInCurProc + uKernelBaseInTargetProc;
	printf("[*] 目标进程中 LoadLibraryA Addr = 0x%p\n",uLoadLibraryAddrInTargetProc);

	//2.获取目标进程中的线程列表
    THREADENTRY32 te32 = {sizeof(THREADENTRY32)} ;
	DWORD dwTidList[1024]={0};
	DWORD Index = 0 ;
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0) ;  
    if (hThreadSnap == INVALID_HANDLE_VALUE)   
        return FALSE ;   
	
    bStatus = FALSE ; 
	printf("[*] 开始枚举目标进程中的线程.\n");
    // 枚举所有线程  
    if (Thread32First(hThreadSnap, &te32))  
    {  
        do{  
            // 判断是否目标进程中的线程  
            if (te32.th32OwnerProcessID == dwPid)  
            {
				bStatus = TRUE;
                dwTidList[Index++] = te32.th32ThreadID;
            }   
			
        }while (Thread32Next ( hThreadSnap, &te32 )) ;  
    }  
	
    CloseHandle (hThreadSnap) ;  
	
	if (!bStatus)
	{
		printf("[-] 无法得到目标进程的线程列表!\n");
		return FALSE;
	}
	printf("[*] 线程枚举完毕，共有 %d 个线程.\n",Index);

	//3. 打开目标进程  ，申请内存，写入Shellcode和参数
	ULONG_PTR uDllPathAddr = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid) ;  
	if (hProcess == NULL)
	{
		printf("[-] 无法打开目标进程!\n");
		return FALSE;
	}

	//3.依次打开线程，获取Context
	bStatus = FALSE ;
	CONTEXT Context;
	ULONG_PTR uEIP = 0 ;
	for (i=0;i<Index;i++)
	{
		hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,dwTidList[i]);
		if (hThread == NULL)
		{
			printf("[-] 打开线程 %d 失败!\n",dwTidList[i]);
			continue;
		}
		
		printf("[*] 打开线程 %d 成功.\n",dwTidList[i]);

		//暂停线程
		DWORD dwSuspendCnt = SuspendThread(hThread);
		if (dwSuspendCnt == (DWORD)-1)
		{
			printf("[-] 暂停线程 %d 失败!\n",dwTidList[i]);
			CloseHandle(hThread);
			continue;
		}
		
		printf("[*] 暂停线程 %d 成功 Cnt = %d.\n",dwTidList[i],dwSuspendCnt);
		//获取Context
		ZeroMemory(&Context,sizeof(CONTEXT));
		Context.ContextFlags = CONTEXT_FULL;
		if (!GetThreadContext(hThread,&Context))
		{
			printf("[-] 无法获取线程 %d 的Context!\n",dwTidList[i]);
			CloseHandle(hThread);
			continue;
		}
		
#ifdef _WIN64
		uEIP = Context.Rip;
#else
		uEIP = Context.Eip;
#endif
		printf("[*] 获取线程 %d 的Context成功 EIP = 0x%p\n",dwTidList[i],uEIP);
		
		// 分配空间  
        lpData = (PBYTE)VirtualAllocEx(hProcess, NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (lpData == NULL)
		{
			printf("[-] 在目标进程申请内存失败!\n");
			CloseHandle(hThread);
			continue;
		}
		
		printf("[*] 在目标进程中申请内存成功, lpData = 0x%p\n",lpData);

		//构造ShellCode
		ZeroMemory(&Data,sizeof(INJECT_DATA));
		PrepareShellCode(Data.ShellCode);
		lstrcpy(Data.szDllPath,szDllFullPath); //Dll路径
		Data.AddrofLoadLibraryA = uLoadLibraryAddrInTargetProc; //LoadLibraryA的地址
		Data.OriginalEIP = uEIP; //原始的EIP地址
		Data.lpDllPath = lpData + FIELD_OFFSET(INJECT_DATA,szDllPath) ; //szDllPath在目标进程中的位置
		printf("[*] ShellCode填充完毕.\n");

		//向目标进程写入ShellCode
		if (!WriteProcessMemory(hProcess, lpData, &Data, sizeof(INJECT_DATA), &dwRet))
		{
			printf("[-] 向目标进程写入ShellCode失败!\n");
			CloseHandle(hThread);
			CloseHandle(hProcess);
			return FALSE;
		}

		printf("[*] 向目标进程写入ShellCode成功.\n");
		
		//设置线程的Context,使EIP指向ShellCode起始地址
#ifdef _WIN64
		Context.Rip = (ULONG_PTR)lpData;
#else
		Context.Eip = (ULONG_PTR)lpData;
#endif
		//设置Context
		if (!SetThreadContext(hThread,&Context))
		{
			printf("[-] 无法设置线程 %d 的Context!\n",dwTidList[i]);
			CloseHandle(hThread);
			continue;
		}

		printf("[*] 设置线程 %d 的Context成功.\n",dwTidList[i]);

		//恢复线程执行
		dwSuspendCnt = ResumeThread(hThread);	
		if (dwSuspendCnt == (DWORD)-1)
		{
			printf("[-] 恢复线程 %d 失败!\n",dwTidList[i]);
			CloseHandle(hThread);
			continue;
		}
		
		printf("[*] 恢复线程 %d 成功. Cnt = %d\n",dwTidList[i],dwSuspendCnt);

		bStatus = TRUE;
		CloseHandle(hThread);

		//Sleep一段时间，继续对下一线程操作，以确保成功率
		Sleep(1000);


	}

	CloseHandle(hProcess);
	printf("[*] 操作全部完毕.\n");

	return bStatus;	
}

ULONG_PTR GetModuleHandleInProcess( DWORD processID , char *szModuleNameToFind )
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;
	char *pCompare = NULL ;
	ULONG_PTR uResult = 0 ;
	
    // Print the process identifier.
	
    printf( "\nProcess ID: %u\n", processID );
	
    // Get a list of all the modules in this process.
	
    hProcess = OpenProcess( 
		PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,
		FALSE, 
		processID 
		);
    if (NULL == hProcess)
        return 0;
	
    if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
            char szModName[MAX_PATH];
			
            // Get the full path to the module's file.
			
            if ( GetModuleFileNameEx(hProcess, hMods[i], szModName,
				sizeof(szModName)/sizeof(char)))
            {
                // Print the module name and handle value.
				pCompare = strrchr(szModName,'\\');
				pCompare = (pCompare == NULL) ? szModName:(pCompare+1);
				if (stricmp(pCompare,szModuleNameToFind) == 0)
				{
					uResult = (ULONG_PTR)hMods[i];
					break;
				}
            }
        }
    }
	
    CloseHandle( hProcess );

	return uResult;
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