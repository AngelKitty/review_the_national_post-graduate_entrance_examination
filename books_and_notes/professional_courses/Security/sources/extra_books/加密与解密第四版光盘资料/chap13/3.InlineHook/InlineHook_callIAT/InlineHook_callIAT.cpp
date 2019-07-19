/////////////////////////////////////////////////////////////////
//         第13章  Hook技术 《加密与解密（第四版）》           //
//                                                             //
//         Author: achillis(黑月教主)                          //
//         Blog  : http://www.cnblogs.com/achillis/            //
//         QQ    : 344132161                                   //
//         Email : achillis@126.com                            //
//         转载请保留作者信息                                  //
//         (c)  看雪学院 www.kanxue.com 2000-2018              //
/////////////////////////////////////////////////////////////////
//程序功能:Hook本进程的TestHook函数对VirtualAlloc函数的调用

#include <windows.h>
#include <stdio.h>
#include <CONIO.H>

ULONG g_PointerToHook = 0 ;
ULONG g_RawIATThunk = 0 ;

LPVOID GetAddress(char *,char *);
LPVOID WINAPI DetourVirtualAlloc(
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType, // type of allocation
	DWORD flProtect   // type of access protection
	);
BOOL Inline_InstallHook(void);
BOOL Inline_UnInstallHook();
VOID TestHook();

int main(int argc, char* argv[])
{
	Inline_InstallHook();
	TestHook();
	Inline_UnInstallHook();
	getchar();
	return 0;
}

VOID TestHook()
{
	LPVOID lpAddr = VirtualAlloc(NULL,0x1000,MEM_COMMIT,PAGE_READWRITE);
}

LPVOID WINAPI DetourVirtualAlloc(
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType,// type of allocation
	DWORD flProtect   // type of access protection
	)
{	LPVOID lpAddr = NULL ;
	printf("[DetourVirtualAllocEx] VirtualAllocEx called.\n");
	lpAddr = VirtualAlloc(lpAddress,dwSize,flAllocationType,flProtect);
	printf("[DetourVirtualAllocEx] Alloced Buffer = 0x%X\n",lpAddr);
	return lpAddr;
}

/*
00401040  /$  6A 04              push 4                                                            ; /Protect = PAGE_READWRITE
00401042  |.  68 00100000        push 1000                                                         ; |AllocationType = MEM_COMMIT
00401047  |.  68 00100000        push 1000                                                         ; |Size = 1000 (4096.)
0040104C  |.  6A 00              push 0                                                            ; |Address = NULL
0040104E  |.  FF15 00604000      call dword ptr ds:[<&KERNEL32.VirtualAlloc>]                      ; \VirtualAlloc
00401054  \.  C3                 retn  
*/
BOOL Inline_InstallHook()
{
	//准备Hook
	BOOL bFound = FALSE;
	BOOL bResult = FALSE ;
	ULONG addrTemp = 0 ;
	ULONG addrTargetFun = (ULONG)GetAddress("kernel32.dll","VirtualAlloc");
	PBYTE pFun = (PBYTE)TestHook;
	PULONG pOldThunk = NULL ;
	int i = 0 ;
	for (i=0;i<0x30;i++,pFun++)
	{
		//FF15 00604000      call dword ptr ds:[<&KERNEL32.VirtualAlloc>] 
		if (pFun[0] == 0xFF && pFun[1] == 0x15) //call dword ptr:ds[xxxxx]
		{
			pOldThunk = (PULONG)*(ULONG*)(pFun + 2 );
			if (*pOldThunk == addrTargetFun) //判断目标函数是不是VirtualAlloc
			{
				bResult = TRUE;
				break;
			}
		}
	}
	
	if (bResult)
	{
		//开始Hook
		g_PointerToHook = (ULONG)pFun + 2 ;
		g_RawIATThunk = *(ULONG*)(pFun+2) ;
		PULONG pNewThunk = (PULONG)malloc(sizeof(ULONG));//申请一块内存，作为ThunkData使用
		*pNewThunk = (ULONG)DetourVirtualAlloc;

		addrTemp = (ULONG)pNewThunk;
		bResult = WriteProcessMemory(GetCurrentProcess(),(LPVOID)g_PointerToHook,&addrTemp,sizeof(LONG),NULL);
	}

	return bResult;
}


BOOL Inline_UnInstallHook()
{
	DWORD addrTemp = g_RawIATThunk ;
	return WriteProcessMemory(GetCurrentProcess(),(LPVOID)g_PointerToHook,&addrTemp,sizeof(LONG),NULL);
}


//获取指定模块中指定API的地址
LPVOID GetAddress(char *dllname,char *funname)
{
	HMODULE hMod=0;
	if (hMod=GetModuleHandle(dllname))
	{
		return GetProcAddress(hMod,funname);
	} 
	else
	{
		hMod=LoadLibrary(dllname);
		return GetProcAddress(hMod,funname);
	}
	
}