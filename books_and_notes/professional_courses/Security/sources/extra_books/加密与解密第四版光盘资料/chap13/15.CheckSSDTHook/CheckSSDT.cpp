/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// CheckSSDT.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "ntapi.h"
#include "PELoader.h"

#pragma comment(lib,"ntdll.lib")

#pragma pack(1)    //SSDT表的结构
typedef struct _ServiceDescriptorEntry {
	DWORD *ServiceTableBase;
	DWORD *ServiceCounterTableBase; //Used only in checked build
	DWORD NumberOfServices;
	BYTE *ParamTableBase;
} ServiceDescriptorTableEntry, *PServiceDescriptorTableEntry;
#pragma pack()

typedef struct _SSDTINFO 
{
	DWORD ServiceIndex;//服务索引
	DWORD CurrentAddr;//当前SSDT表中的地址
	DWORD OriginalAddr;//原始地址
	BOOL bHooked;//是否被Hook了
	char FuncName[124];//函数名称
}SSDTINFO,*PSSDTINFO;


//ZwAccessCheckByTypeResultListAndAuditAlarmByHandle

void PrintZwError( char *funcname, NTSTATUS status );
VOID CheckSSDT();
NTSTATUS ReadKernelMemory(LPVOID BaseAddress,LPVOID lpBuffer,DWORD nSize,LPDWORD lpNumberOfBytesRead);
void PrintZwError( char *funcname, NTSTATUS status );
BOOL GetOSKrnlInfo(
	OUT char *szKrnlPath,
	OUT DWORD *pKernelBase
	);

BOOL EnableDebugPrivilege();
BOOL BuildServiceNameTable(PELoader *pModule);
VOID ShowSSDT();

//存储SSDT信息
SSDTINFO g_SSDTInfo[1000];
//总服务数
DWORD g_dwServiceCnt = 0 ;
//被Hook的服务数
DWORD g_dwHookedCnt = 0 ;

int main(int argc, char* argv[])
{
	EnableDebugPrivilege();
	CheckSSDT();
	return 0;
}

VOID CheckSSDT()
{
	BOOL bResult = FALSE ;
	DWORD dwBytesReturn = 0 ;
	DWORD i = 0 ;
	//////////////////////////////////////////////////////////////////////////
	//1.加载ntdll.dll
	//////////////////////////////////////////////////////////////////////////
	PELoader *ldNtdll = new PELoader;
	char szNtdllPath[MAX_PATH]={0};
	PBYTE pNewLoadedNtdll = NULL ;
	GetSystemDirectory(szNtdllPath,MAX_PATH);
	lstrcat(szNtdllPath,"\\ntdll.dll");

	pNewLoadedNtdll = ldNtdll->LoadPE(szNtdllPath,FALSE,0);
	printf("[*] 重新加载后的ntdll  BaseAddress = 0x%08X\n",pNewLoadedNtdll);
	
	//2.构建函数表
	BuildServiceNameTable(ldNtdll);
	printf("[*] 系统服务名称列表构建完毕.\n");
	delete ldNtdll;
	//ShowSSDT();

	//3.获取当前内核文件的路径和基址
	char szOskrnlPath[MAX_PATH]={0};
	DWORD TrueKernelBase = 0 ;
	bResult = GetOSKrnlInfo(szOskrnlPath,&TrueKernelBase);
	if (!bResult)
	{
		printf("[-] 无法获取系统内核文件的路径和加载基址!\n");
		return ;
	}
	printf("[*] 内核路径 : %s\n",szOskrnlPath);
	printf("[*] 内核基址 : 0x%08X\n",TrueKernelBase);
	
	//4.加载当前内核文件ntoskrnl.exe
	PELoader *ldOsKrnl = new PELoader;
	PBYTE pRemapedOsKrnl = ldOsKrnl->LoadPE(szOskrnlPath,TRUE,TrueKernelBase);
	printf("[*] 重新加载后的OSKrnl  BaseAddress = 0x%08X\n",pRemapedOsKrnl);
	
	ServiceDescriptorTableEntry *pRemapedSDT,*pOriginalSDT,OriginalSDT;
	//获取SSDT的地址
	pRemapedSDT = (ServiceDescriptorTableEntry*)ldOsKrnl->_GetProcAddress(pRemapedOsKrnl,"KeServiceDescriptorTable");
	printf("[*] SSDT In ReMaped OSKrnl : 0x%08X\n",pRemapedSDT);
	//获取到的重加载后的KeServiceDescriptorTable结构是空的，需要根据偏移计算真实的SSDT在内存中的位置

	pOriginalSDT = (ServiceDescriptorTableEntry*)((DWORD)pRemapedSDT - (DWORD)pRemapedOsKrnl + TrueKernelBase );
	printf("[*] SSDT In True    OSKrnl : 0x%08X\n",pOriginalSDT);
	
	//////////////////////////////////////////////////////////////////////////
	//3.读取当前SSDT地址
	//////////////////////////////////////////////////////////////////////////
	printf("[*] 读取 KeServiceDescriptorTable.\n");

	ZeroMemory(&OriginalSDT,sizeof(ServiceDescriptorTableEntry));
	ReadKernelMemory(pOriginalSDT,&OriginalSDT,sizeof(ServiceDescriptorTableEntry),&dwBytesReturn);
	if (dwBytesReturn == 0)
	{
		printf("[-] 读取 KeServiceDescriptorTable 失败!\n");
		return ;
	}

	printf("[*] KiServiceTable = 0x%08X\n",OriginalSDT.ServiceTableBase);
	
	DWORD OriginalKiServiceTable[1000];
	DWORD RemapedKiServiceTable[1000];

	//读取KiServiceTable
	ReadKernelMemory((LPVOID)OriginalSDT.ServiceTableBase,
		OriginalKiServiceTable,
		OriginalSDT.NumberOfServices * sizeof(DWORD) , 
		&dwBytesReturn);

	if (dwBytesReturn == 0)
	{
		printf("[-] 读取 KiServiceTable 失败!\n");
		return ;
	}
	
	for (i=0;i<g_dwServiceCnt;i++)
	{
		g_SSDTInfo[i].CurrentAddr = OriginalKiServiceTable[i];
	}

	//////////////////////////////////////////////////////////////////////////
	//4.读取原始SSDT地址
	//////////////////////////////////////////////////////////////////////////
	DWORD dwKiServiceTableInRemapedKrnl = (DWORD)OriginalSDT.ServiceTableBase - TrueKernelBase + (DWORD)pRemapedOsKrnl;
	printf("[*] 重载后的内核中 KiServiceTable = 0x%08X\n",dwKiServiceTableInRemapedKrnl);

	memcpy(RemapedKiServiceTable,(PVOID)dwKiServiceTableInRemapedKrnl,OriginalSDT.NumberOfServices * sizeof(DWORD));
	for (i=0;i<g_dwServiceCnt;i++)
	{
		g_SSDTInfo[i].OriginalAddr = RemapedKiServiceTable[i];
	}
	//////////////////////////////////////////////////////////////////////////
	//5.对比结果
	//////////////////////////////////////////////////////////////////////////
	g_dwHookedCnt = 0 ;
	for (i=0;i<g_dwServiceCnt;i++)
	{
		if ( g_SSDTInfo[i].OriginalAddr != g_SSDTInfo[i].CurrentAddr)
		{
			g_SSDTInfo[i].bHooked = TRUE ;
			g_dwHookedCnt++;
		}
	}

	printf("[*] 显示检查结果:\n\n");
	ShowSSDT();
	printf("[*] 检查完毕. 共有服务 %d 个，被Hook服务 %d 个。\n",g_dwServiceCnt,g_dwHookedCnt);	
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

NTSTATUS ReadKernelMemory(LPVOID BaseAddress,LPVOID lpBuffer,DWORD nSize,LPDWORD lpNumberOfBytesRead)
{
	SYSDBG_VIRTUAL sysvir;
	sysvir.Address=BaseAddress;
	sysvir.Buffer=lpBuffer;
	sysvir.Request=nSize;
	return ZwSystemDebugControl(SysDbgReadVirtual,//8
		(PVOID)&sysvir,
		sizeof(SYSDBG_VIRTUAL),
		NULL,
		NULL,
		lpNumberOfBytesRead);
	
}

BOOL GetOSKrnlInfo(
	OUT char *szKrnlPath,
	OUT DWORD *pKernelBase)
{
	BOOL bResult = FALSE ;
	PSYSTEM_MODULE_INFORMATION_ENTRY pSysModuleInfo;
	NTSTATUS status=0;
	char *pBuf=NULL;
	DWORD needlen=0,truelen;
	DWORD Modcnt=0;
	
	ZwQuerySystemInformation(SystemModuleInformation,NULL,0,&needlen);
	//printf("Need length:0x%x\n",needlen);
	truelen=needlen;
	status=ZwAllocateVirtualMemory(NtCurrentProcess(),(PVOID*)&pBuf,0,&truelen,MEM_COMMIT,PAGE_READWRITE);
	if (!NT_SUCCESS(status))
	{
		PrintZwError("ZwAllocateVirtualMemory",status);
		return FALSE;
	}
	//printf("Buf:0x%08x\n",pBuf);
	status=ZwQuerySystemInformation(SystemModuleInformation,(PVOID)pBuf,truelen,&needlen);
	if (!NT_SUCCESS(status))
	{
		PrintZwError("ZwQuerySystemInformation",status);
		return FALSE;
	}
	Modcnt=*(DWORD*)pBuf;
	pSysModuleInfo=(PSYSTEM_MODULE_INFORMATION_ENTRY)(pBuf+sizeof(DWORD));
	if (strstr(
		(strlwr(pSysModuleInfo->ImageName),pSysModuleInfo->ImageName),"nt"))
	{
		*pKernelBase=(DWORD)pSysModuleInfo->Base;
		GetSystemDirectory(szKrnlPath,MAX_PATH);
		lstrcat(szKrnlPath,strrchr(pSysModuleInfo->ImageName,'\\'));
	}
	status=ZwFreeVirtualMemory(NtCurrentProcess(),(PVOID*)&pBuf,&truelen,MEM_RELEASE);
	
	return TRUE;
}

BOOL BuildServiceNameTable(PELoader *pModule)
{
	DWORD i =0 ;
	DWORD nFunCnt = 0 ;
	char *szFunName = NULL;
	DWORD *namerav,*funrav;
	PBYTE ModuleBase = pModule->m_hModule ;
	DWORD dwServiceIndex = 0 ;
	PBYTE pFunAddr = NULL ;
	PIMAGE_EXPORT_DIRECTORY pExportDir = pModule->m_pExportDir;
	namerav=(DWORD*)(ModuleBase+pExportDir->AddressOfNames);
	funrav=(DWORD*)(ModuleBase+pExportDir->AddressOfFunctions);

	g_dwServiceCnt = 0 ;
	for (i=0;i<pExportDir->NumberOfNames;i++)
	{
		szFunName = (char*)ModuleBase + namerav[i] ;
		//使用Zw而不使用Nt的原因是有些Nt开头的不是系统服务函数
		if (memcmp(szFunName,"Zw",2) == 0)
		{
			pFunAddr = ModuleBase + funrav[i];
			dwServiceIndex = *(DWORD*)(pFunAddr + 1);
			//保存信息到表格中
			g_SSDTInfo[dwServiceIndex].ServiceIndex = dwServiceIndex;
			lstrcpy(g_SSDTInfo[dwServiceIndex].FuncName,szFunName);
			memcpy(g_SSDTInfo[dwServiceIndex].FuncName,"Nt",2);//改一下函数名字
			g_dwServiceCnt++ ;
		}
		
	}
	return TRUE;
}

VOID ShowSSDT()
{
	DWORD i = 0 ;
	printf(" 索引 原始地址  当前地址 Hook 服务名称\n");
	printf("=========================================================\n");
	for (i=0;i<g_dwServiceCnt;i++)
	{
		printf("[%3d] %08X  %08X %s   %s\n",
			g_SSDTInfo[i].ServiceIndex,
			g_SSDTInfo[i].OriginalAddr,
			g_SSDTInfo[i].CurrentAddr,
			g_SSDTInfo[i].bHooked ? "是":"  ",
			g_SSDTInfo[i].FuncName);
	}
	printf("=========================================================\n");
}

void PrintZwError( char *funcname, NTSTATUS status )
{
    char *errMsg;
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		RtlNtStatusToDosError(status),
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		( LPTSTR )&errMsg, 0, NULL );
    printf("%s ErrCode=0x%08x:%s", funcname,status, errMsg);
    LocalFree( errMsg );
    return;
}