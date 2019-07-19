/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// CreateWith.cpp : Defines the entry point for the console application.
// Tested On:WinXP SP3/Win7 x86/Win7 x64

#include "stdafx.h"
#include "Image.h"
#include <stdlib.h>
#include <Psapi.h>
#include <tlhelp32.h>

#pragma comment(lib,"psapi.lib")

typedef BOOL 
(WINAPI *PDETOUR_CREATE_PROCESS_ROUTINE)(
	LPCTSTR lpApplicationName,// pointer to name of executable module
	LPTSTR lpCommandLine,  // pointer to command line string
	LPSECURITY_ATTRIBUTES lpProcessAttributes,  // process security attributes
	LPSECURITY_ATTRIBUTES lpThreadAttributes,   // thread security attributes
	BOOL bInheritHandles,  // handle inheritance flag
	DWORD dwCreationFlags, // creation flags
	LPVOID lpEnvironment,  // pointer to new environment block
	LPCTSTR lpCurrentDirectory,   // pointer to current directory name
	LPSTARTUPINFO lpStartupInfo,  // pointer to STARTUPINFO
	LPPROCESS_INFORMATION lpProcessInformation  // pointer to PROCESS_INFORMATION
	);

BOOL DetourCreateProcessWithDll(
	LPCTSTR lpApplicationName,
	LPTSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCTSTR lpCurrentDirectory,
	LPSTARTUPINFO lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation,
	LPCSTR lpDllName,
	PDETOUR_CREATE_PROCESS_ROUTINE pfCreateProcess
	);

DWORD ProcesstoPid(char *Processname);
ULONG_PTR FindImageBase(HANDLE hProc,LPSTR lpCommandLine);


int main(int argc, char* argv[])
{
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

#ifdef _WIN64
	DetourCreateProcessWithDll(
		NULL,
		"F:\\Program2016\\DllInjection\\HostProc64.exe",
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi,
		"MsgDll64.dll",
		NULL);
#else
	DetourCreateProcessWithDll(
		NULL,
		//"D:\\killvirus\\PCHunter_free\\PCHunter32.exe",
		"F:\\Program2016\\DllInjection\\HostProc.exe",
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi,
		"MsgDll.dll",
		NULL);
#endif
	return 0;
}



BOOL DetourCreateProcessWithDll(
	LPCTSTR lpApplicationName,
	LPTSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCTSTR lpCurrentDirectory,
	LPSTARTUPINFO lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation,
	LPCSTR szDllName,
	PDETOUR_CREATE_PROCESS_ROUTINE pfCreateProcess
	)
{
	CImage Image;
	LONG Status = 0 ;
	BOOL bResult = FALSE ;
	WORD i = 0 ;
	SIZE_T dwIoCnt = 0 ;
	char szErrMsg[1024]={0};
	PIMAGE_SECTION_HEADER pImpSecHeader,pNewSecHeader = NULL,pTargetSecHeader = NULL;
	DWORD dwOldIIDCnt = 0 ,dwNewIIDCnt = 0 ;
	DWORD dwOldIIDSize = 0, dwNewIIDSize = 0 ;
	DWORD dwVAToStoreNewIID = 0 ; //新IID数组的存储位置
	BOOL bUseOriginalSection = FALSE ; //是否使用了新节
	PROCESS_INFORMATION pi={0};
	ULONG_PTR ProcImageBase = 0 ;
	char szDllExportFunName[]= "Msg";
	HANDLE hFile  = INVALID_HANDLE_VALUE;
	PBYTE ImageBase = 0;
	DWORD dwnewThunkDataSize = 0 ; //新IID项的ThunkData的存储位置
	DWORD dwNewThunkDataVA = 0 ;//新IID项的ThunkData的存储位置
	DWORD dwSizeNeed = 0 ;
	DWORD dwThunkDataOffsetByIID = 0 ;
	BYTE IIDThunkBuffer[0x200]={0};
	DWORD dwOldProtect = 0 ;
	
	printf("[*] Path = %s\n",lpCommandLine);
	if (pfCreateProcess == NULL)
	{
		pfCreateProcess = CreateProcessA;
	}
	
	DWORD dwNewCreationFlags = dwCreationFlags + CREATE_SUSPENDED ;
	
	bResult = pfCreateProcess(
		lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttributes,
		bInheritHandles,
		dwNewCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		lpStartupInfo,
		&pi
		);
	
	//检查是否被感染过
	if (!bResult)
	{
		printf("[-] 创建进程 %s 失败!\n",lpCommandLine);
		return FALSE;
	}
	
	
	//进程创建成功，获取ImageBase并读取PE头
	//由于此时进程内Ldr链表还没建立，所以无法使用EnumProcessModule
	//并且在启用ASLR的情况下，进程不一定能够加载在PE头中指定的ImageBase处
	ImageBase = (PBYTE)FindImageBase(pi.hProcess,lpCommandLine);
	if (ImageBase == NULL)
	{
		printf("[-] 无法获取进程的ImageBase!\n",lpCommandLine);
		return FALSE;
	}

	printf("[*] ImageBase = 0x%p\n",ImageBase);
	
	//读取目标进程的PE头
	bResult = Image.AttachToProcess(pi.hProcess,ImageBase);
	if (!bResult)
	{
		printf("[-] 读取目标进程exe映像的PE头失败!\n");
		return FALSE;
	}
	printf("[*] PE头读取成功!\n");
	printf("[*] 当前导入表信息 VA = 0x%p Size = 0x%X\n",Image.m_pImpDataDir->VirtualAddress,Image.m_pImpDataDir->Size);
	dwOldIIDSize = Image.m_pImpDataDir->Size ;
	dwOldIIDCnt = dwOldIIDSize / sizeof(IMAGE_IMPORT_DESCRIPTOR) ;
	dwNewIIDCnt = dwOldIIDCnt + 1;
	dwNewIIDSize = dwNewIIDCnt * sizeof(IMAGE_IMPORT_DESCRIPTOR) ;
	printf("[*] dwOldIIDCnt = %d  Size = 0x%X\n",dwOldIIDCnt,dwOldIIDSize);
	printf("[*] dwNewIIDCnt = %d  Size = 0x%X\n",dwNewIIDCnt,dwNewIIDSize);
	
	pImpSecHeader = Image.LocateSectionByRVA(Image.m_pImpDataDir->VirtualAddress);
	printf("[*] 导入表所在节  %s  RawOffset = 0x%X Size = 0x%X\n",
		pImpSecHeader->Name,
		pImpSecHeader->PointerToRawData,
		pImpSecHeader->SizeOfRawData);
	
	
	dwnewThunkDataSize = sizeof(ULONG_PTR)*4 + strlen(szDllName) + 1 + sizeof(WORD) + strlen(szDllExportFunName) + 1 ;
	dwThunkDataOffsetByIID = ALIGN_SIZE_UP(dwNewIIDSize,sizeof(ULONG_PTR)) ;

	//所需的大小是新导入表IID结构的大小加上ThunkData的大小
	dwSizeNeed = dwThunkDataOffsetByIID + dwnewThunkDataSize ;
	//由于都是在内存中操作，不必考虑原来文件中的大小，所以直接申请新节
	pNewSecHeader = Image.AddNewSectionToMemory(".Patch",dwSizeNeed);
	printf("[*] 新节添加完毕!\n");
	printf("[*] NewSection VA = 0x%X VirtualSize = 0x%X RawSize = 0x%X\n",
		ImageBase + pNewSecHeader->VirtualAddress ,pNewSecHeader->Misc.VirtualSize,pNewSecHeader->SizeOfRawData);
	
	dwVAToStoreNewIID = pNewSecHeader->VirtualAddress ;
	pTargetSecHeader = pNewSecHeader;

	//保存原导入表
	PIMAGE_IMPORT_DESCRIPTOR pOldImpDesp = Image.m_pImportDesp;
	PIMAGE_IMPORT_DESCRIPTOR pBuildNewImpDesp = (PIMAGE_IMPORT_DESCRIPTOR)malloc(dwSizeNeed);
	ZeroMemory(pBuildNewImpDesp,dwSizeNeed);

	//保存原来的导入表
	bResult = ReadProcessMemory(pi.hProcess,
		ImageBase + Image.m_pImpDataDir->VirtualAddress,
		pBuildNewImpDesp,
		dwOldIIDSize,
		&dwIoCnt);

	if (!bResult)
	{
		printf("[-] 读取目标进程原始导入表失败!\n");
		return FALSE;
	}
	//memcpy(pBuildNewImpDesp,pOldImpDesp,dwOldIIDSize);
	printf("[*] 原导入表IID结构保存完毕.\n");

	//指向一个新添加的IID项,稍后填充
	PIMAGE_IMPORT_DESCRIPTOR pNewImpEntry = pBuildNewImpDesp + dwOldIIDCnt - 1;
	//准备填充新导入表项的Thunks信息
	PULONG_PTR pOriginalFirstThunk = (PULONG_PTR)IIDThunkBuffer;
	ZeroMemory(pOriginalFirstThunk,dwOldIIDSize);
	//留出两项内容，第一项稍后填充，第二项填0作为结束标记
	PULONG_PTR pFirstThunk = pOriginalFirstThunk + 2 ;
	//留出两项内容,第一项稍后填充，第二项填0作为结束标记，之后作为Dll名称
	PCHAR  pDllName = (PCHAR)(pFirstThunk + 2);
	//保存dll名称
	strcpy(pDllName,szDllName);
	
	SIZE_T DllNameLen = strlen(szDllName);
	pDllName[DllNameLen] = 0;
	//接下来作为一个PIMPORT_BY_NAME结构
	PIMAGE_IMPORT_BY_NAME pImpName = (PIMAGE_IMPORT_BY_NAME)(pDllName + ALIGN_SIZE_UP(DllNameLen + 1,sizeof(ULONG_PTR)));
	//填充它
	pImpName->Hint = 0;
	strcpy((char*)pImpName->Name,szDllExportFunName);
	printf("[*] 新导入表IID子结构构造完毕.\n");
	
	//计算结束位置
	PBYTE pEnd = (PBYTE)pImpName + sizeof(pImpName->Hint) + strlen((char*)pImpName->Name) + 1;
	//计算总占用的空间大小
	DWORD dwNewIIDEntrySizeUsed = (DWORD)(pEnd - (PBYTE)pOriginalFirstThunk);
	printf("[*] 新IID成员占用的空间大小 = 0x%X\n",dwNewIIDEntrySizeUsed);
	
	dwNewThunkDataVA = pNewSecHeader->VirtualAddress + dwThunkDataOffsetByIID ;
	//返过来填充OriginalFirstThunk和FirstThunk
	//根据定义，OriginalFirst应指向IMAGE_IMPORT_BY_NAME结构的偏移
	//计算ImpName相对于当前ThunkBuffer的偏移，而ThunkBuffer的位置实际是在原进程的导入表IID区域
	pOriginalFirstThunk[0] = (DWORD)(MEM_OFFSET(pImpName,IIDThunkBuffer) + dwNewThunkDataVA);
	pFirstThunk[0] = pOriginalFirstThunk[0];

	//最后填充新的IID项，计算各项的RVA，应计算出相对于当前Buffer的内部偏移，再加上原来的真正的IID起始偏移
	//也就是要存放Thunk结构的地方
	
	pNewImpEntry->OriginalFirstThunk = (DWORD)(MEM_OFFSET(pOriginalFirstThunk,IIDThunkBuffer) + dwNewThunkDataVA);
	pNewImpEntry->Name = (DWORD)(MEM_OFFSET(pDllName,IIDThunkBuffer) + dwNewThunkDataVA);
	pNewImpEntry->FirstThunk = (DWORD)(MEM_OFFSET(pFirstThunk,IIDThunkBuffer) + dwNewThunkDataVA);
	printf("[*] 新IID填充完毕.\n");

	//更新PE头中的几个值
	//新的导入表大小
	Image.m_pImpDataDir->Size = dwNewIIDSize;
	//新的导入表IID的起始偏移
	Image.m_pImpDataDir->VirtualAddress = dwVAToStoreNewIID;
	
	//申请的新节是可写的，且ThunkData也在新节中，所以不存在不可写的问题
	//如果需要修改原节的数据，因为此时各节已经映射完毕，所以再修改PE头中的节属性已经无用，需要直接修改映射后的页属性
	//pImpSecHeader->Characteristics |= IMAGE_SCN_MEM_WRITE;
	

	//清空绑定输入表,强迫加载器重新加载IAT
	Image.m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
	Image.m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
	printf("[*] PE头更新完毕.\n");

	//写入文件
	printf("[*] 开始更新内存中的PE数据.\n");

	//开始保存内存中的修改内容到文件中
	//先写入新的PE头
	PBYTE TargetAddress = 0;
	DWORD dwSizeToChange = 0;
	DWORD dwVAInMemory = 0 ;
	TargetAddress = ImageBase;
	printf("[*] 准备写入PE头: StartAddress = 0x%p Size = 0x%X\n",TargetAddress,Image.m_pOptHeader->SizeOfHeaders);
	bResult = VirtualProtectEx(pi.hProcess,TargetAddress,Image.m_pOptHeader->SizeOfHeaders,PAGE_READWRITE,&dwOldProtect);
	if (!bResult)
	{
		sprintf(szErrMsg,"无法修改目标进程中地址 0x%p Size = 0x%X 区域的内存属性!",TargetAddress,Image.m_pOptHeader->SizeOfHeaders);
		Image.FormatErrorMsg(szErrMsg,GetLastError());
		Image.GetErrorMsg(szErrMsg,1024);
		printf("[-] %s\n",szErrMsg);
		return FALSE;
	}
	bResult = WriteProcessMemory(pi.hProcess,ImageBase,Image.m_HeaderData,Image.m_pOptHeader->SizeOfHeaders,&dwIoCnt);
	if (!bResult)
	{
		Image.FormatErrorMsg("向目标进程写入PE头失败!",GetLastError());
		Image.GetErrorMsg(szErrMsg,1024);
		printf("[-] %s\n",szErrMsg);
		return FALSE;
	}
	VirtualProtectEx(pi.hProcess,TargetAddress,Image.m_pOptHeader->SizeOfHeaders,dwOldProtect,&dwOldProtect);

	printf("[*] PE头写入完毕.\n");
	
	//写入新的IID结构,因为都在新申请的内存中(PAGE_EXECUTE_READWRITE)，所以不需要再修改页保护
	dwVAInMemory = (DWORD)Image.m_pImpDataDir->VirtualAddress;
	TargetAddress = ImageBase + dwVAInMemory ;
	bResult = WriteProcessMemory(pi.hProcess,ImageBase + dwVAInMemory,pBuildNewImpDesp,dwNewIIDSize,&dwIoCnt);
	if (!bResult)
	{
		Image.FormatErrorMsg("[-] 向目标进程写入IID失败!",GetLastError());
		return FALSE;
	}

	//写入新IID的子结构信息,位置在新导入表结束之后
	dwVAInMemory = MEM_OFFSET(pOriginalFirstThunk,IIDThunkBuffer) + dwNewThunkDataVA;
	TargetAddress = ImageBase + dwVAInMemory ;
	printf("[*] 准备写入IID信息: StartAddress = 0x%p Size = 0x%X\n",TargetAddress,Image.m_pOptHeader->SizeOfHeaders);
	bResult = WriteProcessMemory(pi.hProcess,ImageBase + dwVAInMemory,pOriginalFirstThunk,dwNewIIDEntrySizeUsed,&dwIoCnt);
	if (!bResult)
	{
		Image.FormatErrorMsg("[-] 向目标进程写入IID Thunk数据失败!",GetLastError());
		return FALSE;
	}
	printf("[*] 新IID项的子结构写入完毕. Add = 0x%X Size = 0x%x\n",dwVAInMemory,dwIoCnt);

	
	printf("[*] 新导入表整体写入完毕. Offset = 0x%X Size = 0x%x\n",dwVAInMemory,dwIoCnt);
	printf("[*] 导入表感染完毕.\n");
	
	if (!(dwCreationFlags & CREATE_SUSPENDED))
	{
		//继续执行主线程
		ResumeThread(pi.hThread);
	}
	
	if (lpProcessInformation != NULL)
	{
		memcpy(lpProcessInformation,&pi,sizeof(PROCESS_INFORMATION));
	}
	
	return TRUE;
}

ULONG_PTR FindImageBase(HANDLE hProc,LPSTR lpCommandLine)
{
	ULONG_PTR uResult = 0 ;
	TCHAR szBuf[1024]={0};
	SIZE_T dwSize = 0 ;
	PBYTE pAddress = NULL ;
	
	MEMORY_BASIC_INFORMATION mbi = {0};
	BOOL bFoundMemImage = FALSE ;
	char szImageFilePath[MAX_PATH]={0};
	char *pFileNameToCheck = strrchr(lpCommandLine,'\\');

	//获取页的大小
	SYSTEM_INFO sysinfo;
	ZeroMemory(&sysinfo,sizeof(SYSTEM_INFO));
	GetSystemInfo(&sysinfo);
	
	//Found First MEM_IMAGE Page
	pAddress = (PBYTE)sysinfo.lpMinimumApplicationAddress;
	while (pAddress < (PBYTE)sysinfo.lpMaximumApplicationAddress)
	{
		ZeroMemory(&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		dwSize = VirtualQueryEx(hProc,pAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		if (dwSize == 0)
		{
			pAddress += sysinfo.dwPageSize ;
			continue;
		}

		switch(mbi.State)
		{
		case MEM_FREE:
		case MEM_RESERVE:
			pAddress = (PBYTE)mbi.BaseAddress + mbi.RegionSize;
			break;
		case MEM_COMMIT:
			if (mbi.Type == MEM_IMAGE)
			{
				if (GetMappedFileName(hProc,pAddress,szImageFilePath,MAX_PATH) != 0)
				{
					//printf("Address = 0x%p ImageFileName = %s\n",pAddress,szImageFilePath);
					char *pCompare = strrchr(szImageFilePath,'\\');
					if (stricmp(pCompare,pFileNameToCheck) == 0)
					{
						bFoundMemImage = TRUE;
						uResult = (ULONG_PTR)pAddress;
						break;
					}
				}
			}
			pAddress = (PBYTE)mbi.BaseAddress + mbi.RegionSize;
		    break;
		default:
		    break;
		}

		if (bFoundMemImage)
		{
			break;
		}
	}
	return uResult ;
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