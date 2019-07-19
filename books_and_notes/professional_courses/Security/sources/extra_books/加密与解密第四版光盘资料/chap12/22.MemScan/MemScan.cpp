/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// MemScan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <Psapi.h>

#pragma comment(lib,"psapi.lib")

VOID ScanProcessMemory(HANDLE hProc);
VOID AllocMem();

int main(int argc, char* argv[])
{
	//申请一块"可疑"内存
	AllocMem();
	//以下代码为扫描本进程，也可以扫描其它进程
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS,FALSE,GetCurrentProcessId());
	ScanProcessMemory(hProc);
	CloseHandle(hProc);
	getchar();
	return 0;
}

VOID ScanProcessMemory(HANDLE hProc)
{
	ULONG_PTR uResult = 0 ;
	TCHAR szBuf[1024]={0};
	SIZE_T stSize = 0 ;
	PBYTE pAddress = (PBYTE)0 ;
	SYSTEM_INFO sysinfo;
	MEMORY_BASIC_INFORMATION mbi = {0};
	char szImageFilePath[MAX_PATH]={0};
	
	//获取页的大小
	ZeroMemory(&sysinfo,sizeof(SYSTEM_INFO));
	GetSystemInfo(&sysinfo);
	
	//Check Mem Status
	printf("   Address  :   Size      State     Type     Image\n");
	pAddress = (PBYTE)sysinfo.lpMinimumApplicationAddress ;
	while (pAddress < (PBYTE)sysinfo.lpMaximumApplicationAddress)
	{
		ZeroMemory(&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		stSize = VirtualQueryEx(hProc,pAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		if (stSize == 0)
		{
			pAddress += sysinfo.dwPageSize ;
			continue;
		}
		
		switch(mbi.State)
		{
		case MEM_FREE:
			printf("[0x%p : 0x%p] Free \n",pAddress,mbi.RegionSize);
			pAddress = (PBYTE)mbi.BaseAddress + mbi.RegionSize;
			break;
		case MEM_RESERVE:
			printf("[0x%p : 0x%p] Reverse \n",pAddress,mbi.RegionSize);
			pAddress = (PBYTE)mbi.BaseAddress + mbi.RegionSize;
			break;
		case MEM_COMMIT:
			printf("[0x%p : 0x%p] Commmit ",pAddress,mbi.RegionSize);
			switch(mbi.Type)
			{
			case MEM_IMAGE:
				printf("Image ");
				if (GetMappedFileName(hProc,pAddress,szImageFilePath,MAX_PATH) != 0)
				{
					printf("Image = %s",szImageFilePath);
				}
				break;
			case MEM_MAPPED:
				printf("Mapped ");
				if (GetMappedFileName(hProc,pAddress,szImageFilePath,MAX_PATH) != 0)
				{
					printf("Image = %s",szImageFilePath);
				}
				break;
			case MEM_PRIVATE:
				printf("Private ");
				if (mbi.Protect == PAGE_EXECUTE_READWRITE)
				{
					printf("<= Maybe Stealth Code ");
				}
			    break;
			default:
			    break;
			}
			printf("\n");
			pAddress = (PBYTE)mbi.BaseAddress + mbi.RegionSize;
			break;
		default:
			break;
		}
	}
}

VOID AllocMem()
{
	LPVOID lpData = VirtualAlloc(NULL,0x3000,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	printf("lpData = 0x%p\n",lpData);
}