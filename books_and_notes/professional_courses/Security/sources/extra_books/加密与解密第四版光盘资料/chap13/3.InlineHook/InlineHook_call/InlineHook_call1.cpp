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

//本程序的功能是对VirtualAlloc函数进行call Hook

#include <windows.h>
#include <stdio.h>
#include <CONIO.H>

ULONG g_PointerToRawData = 0 ;
ULONG g_RawOffset = 0 ;

LPVOID GetAddress(char *,char *);
LPVOID WINAPI DetourVirtualAllocEx(
	HANDLE hProcess,  // process within which to allocate memory
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType,	// type of allocation
	DWORD flProtect   // type of access protection
	);
BOOL Inline_InstallHook(void);
BOOL Inline_UnInstallHook();
VOID TestHook();

int main(int argc, char* argv[])
{
	TestHook();
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

LPVOID WINAPI DetourVirtualAllocEx(
	HANDLE hProcess,  // process within which to allocate memory
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType,							// type of allocation
	DWORD flProtect   // type of access protection
	)
{	LPVOID lpAddr = NULL ;
	printf("[DetourVirtualAllocEx] VirtualAllocEx called.\n");
	lpAddr = VirtualAllocEx(hProcess,lpAddress,dwSize,flAllocationType,flProtect);
	printf("[DetourVirtualAllocEx] Alloced Buffer = 0x%X\n",lpAddr);
	return lpAddr;
}

/*
7C809AF1 >  8BFF                 mov edi,edi
7C809AF3    55                   push ebp
7C809AF4    8BEC                 mov ebp,esp
7C809AF6    FF75 14              push dword ptr ss:[ebp+14]
7C809AF9    FF75 10              push dword ptr ss:[ebp+10]
7C809AFC    FF75 0C              push dword ptr ss:[ebp+C]
7C809AFF    FF75 08              push dword ptr ss:[ebp+8]
7C809B02    6A FF                push -1
7C809B04    E8 09000000          call kernel32.VirtualAllocEx
7C809B09    5D                   pop ebp
7C809B0A    C2 1000              retn 10
*/
BOOL Inline_InstallHook()
{
	//准备Hook
	BOOL bFound = FALSE;
	BOOL bResult = FALSE ;
	ULONG addrTemp = 0 ;
	ULONG addrTargetFun = (ULONG)GetAddress("kernel32.dll","VirtualAllocEx");
	PBYTE pFun = (PBYTE)GetAddress("kernel32.dll","VirtualAlloc");
	int i = 0 ;
	for (i=0;i<0x30;i++,pFun++)
	{
		// 7C809B04    E8 09000000          call kernel32.VirtualAllocEx
		if (pFun[0] == 0xE8)
		{
			addrTemp = (ULONG)pFun + 5 + *(LONG*)(pFun+1);
			if (addrTemp == addrTargetFun) //判断call的目标地址是不是VirtualAllocEx
			{
				bResult = TRUE;
				break;
			}
		}
	}
	
	if (bResult)
	{
		//开始Hook
		g_PointerToRawData = (ULONG)pFun + 1 ;
		g_RawOffset = *(LONG*)(pFun+1) ;
		addrTemp = (LONG)DetourVirtualAllocEx - (LONG)pFun - 5 ;
		bResult = WriteProcessMemory(GetCurrentProcess(),pFun+1,&addrTemp,sizeof(LONG),NULL);
	}

	return bResult;
}


BOOL Inline_UnInstallHook()
{
	DWORD addrTemp = g_RawOffset ;
	return WriteProcessMemory(GetCurrentProcess(),(LPVOID)g_PointerToRawData,&addrTemp,sizeof(LONG),NULL);
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