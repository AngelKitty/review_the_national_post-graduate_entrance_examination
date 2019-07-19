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

//程序功能:对kernel32.dll的导出函数VirtualAllocEx进行InlineHook
//主要演示了对以SEH初始化指令开头的API如何Hook

#include <windows.h>
#include <stdio.h>
#include <CONIO.H>

//定义如下结构，保存一次InlineHook所需要的信息
typedef struct _HOOK_DATA{
	char szApiName[128];	//待Hook的API名字
	char szModuleName[128];	//待Hook的API所属模块的名字
	int  HookCodeLen;		//Hook长度
	BYTE oldEntry[8];		//保存Hook位置的原始指令
	BYTE newEntry[8];		//保存要写入Hook位置的新指令
	BYTE HotPatchCode[8];	//用于HotPatch式Hook
	ULONG_PTR HookPoint;		//待HOOK的位置
	ULONG_PTR JmpBackAddr;		//回跳到原函数中的位置
	ULONG_PTR pfnTrampolineFun;	//调用原始函数的通道
	ULONG_PTR pfnDetourFun;		//HOOK过滤函数
}HOOK_DATA,*PHOOK_DATA;

#define HOOKLEN (7)	//要改写的指令的长度,即jmp eax方式

HOOK_DATA HookData;

ULONG_PTR SkipJmpAddress(ULONG_PTR uAddress);
LPVOID GetAddress(char *,char *);
void makehookentry(PVOID HookPoint);
LPVOID WINAPI DetourVirtualAllocEx(
	HANDLE hProcess,  // process within which to allocate memory
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType,	// type of allocation
	DWORD flProtect   // type of access protection
	);

LPVOID WINAPI TrampolineVirtualAllocEx(
	HANDLE hProcess,  // process within which to allocate memory
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType,	// type of allocation
	DWORD flProtect   // type of access protection
	);

BOOL Inline_InstallHook(void);
BOOL Inline_UnInstallHook();
BOOL InstallCodeHook(PHOOK_DATA pHookData);
BOOL UninstallCodeHook(PHOOK_DATA pHookData);

int main(int argc, char* argv[])
{
	VirtualAllocEx(GetCurrentProcess(),NULL,0x1000,MEM_COMMIT,PAGE_READWRITE);
	Inline_InstallHook();
	printf("Inline Hook Installed.\n");
	VirtualAllocEx(GetCurrentProcess(),NULL,0x2000,MEM_COMMIT,PAGE_READWRITE);
	Inline_UnInstallHook();
	printf("Inline Hook UnInstalled.\n");
	VirtualAllocEx(GetCurrentProcess(),NULL,0x3000,MEM_COMMIT,PAGE_READWRITE);
	return 0;
}

//注意函数的定义和原始函数一定要一样，尤其是调用约定，否则函数返回后将出错
LPVOID WINAPI DetourVirtualAllocEx(
	HANDLE hProcess,  // process within which to allocate memory
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType,
	// type of allocation
	DWORD flProtect   // type of access protection
	)
{
	printf("DetourVirtualAllocEx called.  AllocSize = 0x%X\n",dwSize);
	return TrampolineVirtualAllocEx(hProcess,lpAddress,dwSize,flAllocationType,flProtect);
}

BOOL Inline_InstallHook()
{
	//准备Hook
	ZeroMemory(&HookData,sizeof(HOOK_DATA));
	strcpy(HookData.szApiName , "VirtualAllocEx");
	strcpy(HookData.szModuleName , "kernel32.dll");
	HookData.HookPoint = (ULONG_PTR)GetAddress(HookData.szModuleName,HookData.szApiName);//HOOK的地址
	HookData.pfnTrampolineFun = (ULONG_PTR)TrampolineVirtualAllocEx;//调用原始函数的通道
	HookData.pfnDetourFun = (ULONG_PTR)DetourVirtualAllocEx;//DetourFun
	HookData.HookCodeLen = HOOKLEN;
	return InstallCodeHook(&HookData);
}


BOOL Inline_UnInstallHook()
{
	return UninstallCodeHook(&HookData);
}
/*
VirtualAllocEx的代码开头:
7C809B12 >  6A 10              push 10
7C809B14    68 609B807C        push kernel32.7C809B60
7C809B19    E8 B889FFFF        call kernel32.7C8024D6
*/
//当需要调用原始的VirtualAllocEx时，直接调用此函数即可，参数完全相同
__declspec( naked )
LPVOID WINAPI TrampolineVirtualAllocEx(
	HANDLE hProcess,  // process within which to allocate memory
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType,// type of allocation
	DWORD flProtect   // type of access protection
	)
{
	_asm
	{
		//由于我们写入的Hook指令破坏了原函数开头的几条指令，所以将原来的指令复制到这里，在这里执行原函数的前几条指令
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		jmp HookData.JmpBackAddr //跳到Hook代码之后的地方，绕过自己安装的HOOK
	}
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

/*	
	这里计算一下要填充的指令
	根据要HOOK的长度不同及现场保护的不同要求，可以采用不同的HOOK指令
	通常远跳转最少要5字节，下面是常用的三种情况:

	5字节	jmp xxxxxxxx  (0xE9) 大部分函数开头是这样的
	77D5050B >  8BFF                   mov edi,edi
	77D5050D    55                     push ebp
	77D5050E    8BEC                   mov ebp,esp
	77D50510    833D 1C04D777 00       cmp dword ptr ds:[gfEMIEnable],0

	6字节	push xxxxxxxx/ret 使用情况较少

	7字节	mov eax,xxxxxxxx/jmp eax 常用于有SEH的函数开头,如IsBadReadPtr
	7565D015 >  6A 0C                          push 0C
	7565D017    68 78D06575                    push kernel32.7565D078
	7565D01C    E8 9F45FDFF                    call kernel32.756315C0
	
	当要改变的指令长度大于以上时，可以使用nop来填充
*/
void InitHookEntry(PHOOK_DATA pHookData)
{
	if (pHookData==NULL
		|| pHookData->pfnDetourFun==NULL
		|| pHookData->HookPoint==NULL
		|| pHookData->HookCodeLen == 0)
	{
		return;
	}
	
	switch(pHookData->HookCodeLen)
	{
	case 2://Hot Patch
		{
			pHookData->newEntry[0] = 0xEB; //Jmp -5
			pHookData->newEntry[1] = 0xF9; 
			pHookData->HotPatchCode[0] = 0xE9; //Jmp
			*(ULONG*)(pHookData->HotPatchCode+1) = (ULONG)pHookData->pfnDetourFun - ((ULONG)pHookData->HookPoint - 5)- 5;//0xE9 式jmp的计算
		}
		break;
	case 5:
		{
			pHookData->newEntry[0] = 0xE9; //Jmp 
			//计算跳转偏移并写入
			*(ULONG*)(pHookData->newEntry+1) = (ULONG)pHookData->pfnDetourFun - (ULONG)pHookData->HookPoint - 5;//0xE9 式jmp的计算
		}
		break;
	case 6:
		{
			/*
			0040E9D1 >    68 44332211      push 11223344
			0040E9D6      C3               retn
			*/
			memcpy(pHookData->newEntry,"\x68\x44\x33\x22\x11\xC3",6);
			*(ULONG*)(pHookData->newEntry+1) = (ULONG)pHookData->pfnDetourFun ;
		}
		break;
	case 7:
		/*
		7C809B12 >  B8 44332211        mov eax,11223344
		7C809B17    FFE0               jmp eax
		*/
		{
			memcpy(pHookData->newEntry,"\xB8\x44\x33\x22\x11\xFF\xE0",7);
			*(ULONG*)(pHookData->newEntry+1) = (ULONG)pHookData->pfnDetourFun ;
		}
	    break;
	default:
	    break;
	}
	
	
}

ULONG_PTR SkipJmpAddress(ULONG_PTR uAddress)
{
	ULONG_PTR TrueAddress = 0 ;
	PBYTE pFn = (PBYTE)uAddress;
	
	if (memcmp(pFn,"\xFF\x25",2) == 0)
	{
		TrueAddress = *(ULONG_PTR*)(pFn + 2);
		return TrueAddress;
	}
	
	if (pFn[0] == 0xE9)
	{
		TrueAddress = (ULONG_PTR)pFn + *(ULONG_PTR*)(pFn + 1) + 5 ;
		return TrueAddress;
	}
	
	if (pFn[0] == 0xEB)
	{
		TrueAddress = (ULONG_PTR)pFn + pFn[1] + 2 ;
		return TrueAddress;
	}
	
	return (ULONG_PTR)uAddress;
}

BOOL InstallCodeHook(PHOOK_DATA pHookData)
{
	DWORD dwBytesReturned=0;
	DWORD dwOldProtect;
	PBYTE pfnHead = NULL;
	PBYTE pAddrToWrite = NULL ;
	BOOL bResult=FALSE;
	MEMORY_BASIC_INFORMATION MBI={0};
	
	if (pHookData==NULL
		|| pHookData->HookPoint==0
		|| pHookData->pfnDetourFun==NULL
		|| pHookData->pfnTrampolineFun==NULL)
	{
		return FALSE;
	}

	pHookData->pfnTrampolineFun = SkipJmpAddress(pHookData->pfnTrampolineFun);
	pHookData->HookPoint = SkipJmpAddress(pHookData->HookPoint); //如果函数开头是跳转，那么将其跳过
	pHookData->JmpBackAddr = pHookData->HookPoint + pHookData->HookCodeLen ;
	pfnHead = (PBYTE)pHookData->HookPoint;
	printf("Address To HOOK=0x%08X\n",pfnHead);

	//检查是否被Hook过
	if (memcmp(pfnHead,"\x8B\xFF\x55\x8B\xEC",5) == 0 //push ebp,mov ebp,esp
		|| (pfnHead[0] == 0x6A && pfnHead[2] == 0x68)) //push sehhandler
	{
		//未Hook过才能继续
		InitHookEntry(pHookData);//填充Inline Hook代码
	}
	else
	{
		printf("Addr 0x%X already hooked.\n");
	}
	

	//读取原函数代码
	memcpy(pHookData->oldEntry,(void*)pHookData->HookPoint,8);
	//如果不是HotPatch Hook，需要把原函数的指令替换掉Trampoline中的nop
	if (pHookData->HookCodeLen != 2)
	{
		pAddrToWrite = (PBYTE)pHookData->pfnTrampolineFun ;
		if (VirtualQuery(pAddrToWrite,&MBI,sizeof(MEMORY_BASIC_INFORMATION))
			&& VirtualProtect(MBI.BaseAddress,pHookData->HookCodeLen,PAGE_EXECUTE_READWRITE,&dwOldProtect))
		{
			memcpy(pAddrToWrite,pHookData->oldEntry,pHookData->HookCodeLen);
			VirtualProtect(MBI.BaseAddress,pHookData->HookCodeLen,dwOldProtect,&dwOldProtect);
		}
	}

	//下面开始Hook
	if (pHookData->HookCodeLen == 2)
	{
		pAddrToWrite = (PBYTE)pHookData->HookPoint - 5 ;
	}
	else
	{
		pAddrToWrite = (PBYTE)pHookData->HookPoint;
	}

	if (VirtualQuery(pAddrToWrite,&MBI,sizeof(MEMORY_BASIC_INFORMATION))
		&& VirtualProtect(MBI.BaseAddress,pHookData->HookCodeLen,PAGE_EXECUTE_READWRITE,&dwOldProtect))  //实际上这里会修改整页的属性
	{
		if (pHookData->HookCodeLen == 2)
		{
			//先填充jmp指令
			memcpy(pAddrToWrite,pHookData->HotPatchCode,5);
			pAddrToWrite += 5;
		}
		memcpy(pAddrToWrite,pHookData->newEntry,pHookData->HookCodeLen);
		VirtualProtect(MBI.BaseAddress,pHookData->HookCodeLen,dwOldProtect,&dwOldProtect);
	}

	return bResult;
}

BOOL UninstallCodeHook(PHOOK_DATA pHookData)
{
	DWORD dwBytesReturned=0;
	DWORD dwOldProtect;
	BOOL bResult=FALSE;
	MEMORY_BASIC_INFORMATION MBI={0};
	PBYTE pAddrToWrite = NULL ;
	if (pHookData==NULL
		|| pHookData->HookPoint==0
		|| pHookData->oldEntry[0]==0)
	{
		return FALSE;
	}
	pAddrToWrite=(PBYTE)pHookData->HookPoint;
	if (VirtualQuery(pAddrToWrite,&MBI,sizeof(MEMORY_BASIC_INFORMATION))
		&& VirtualProtect(MBI.BaseAddress,pHookData->HookCodeLen,PAGE_EXECUTE_READWRITE,&dwOldProtect))
	{
		memcpy(pAddrToWrite,pHookData->oldEntry,pHookData->HookCodeLen);
		bResult=TRUE;
		VirtualProtect(MBI.BaseAddress,pHookData->HookCodeLen,dwOldProtect,&dwOldProtect);
	}
	return bResult;
}
