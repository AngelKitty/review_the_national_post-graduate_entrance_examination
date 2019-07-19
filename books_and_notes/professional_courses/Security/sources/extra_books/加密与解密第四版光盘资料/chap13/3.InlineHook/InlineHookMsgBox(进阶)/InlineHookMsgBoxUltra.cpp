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

//程序功能:对user32.dll导出的MessageBoxA进行Inline Hook
//本程序中实现了一个简单的Hook引擎，可以通过更改下面的宏定义实现不同的InlineHook
//对MessageBoxA，可选值为2,5,7

#define HOOKCODELEN 7

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
	ULONG_PTR JmpBackAddr;		//回跳到原函数中的地址
	ULONG_PTR pfnTrampolineFun;	//调用原始函数的通道
	ULONG_PTR pfnDetourFun;		//HOOK过滤函数
}HOOK_DATA,*PHOOK_DATA;

#define HOOKLEN (7)	//要改写的指令的长度

HOOK_DATA MsgBoxHookData;

ULONG_PTR SkipJmpAddress(ULONG_PTR uAddress);
LPVOID GetAddress(char *,char *);
void makehookentry(PVOID HookPoint);
LPVOID DetourVirtualAllocEx(
	HANDLE hProcess,  // process within which to allocate memory
	LPVOID lpAddress, // desired starting address of allocation
	DWORD dwSize,     // size, in bytes, of region to allocate
	DWORD flAllocationType,  // type of allocation
	DWORD flProtect   // type of access protection
	);

int WINAPI TrampolineMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
BOOL Inline_InstallHook(void);
BOOL Inline_UnInstallHook();
BOOL InstallCodeHook(PHOOK_DATA pHookData);
BOOL UninstallCodeHook(PHOOK_DATA pHookData);

int main(int argc, char* argv[])
{
	MessageBoxA(NULL,"Before Inline Hook","Test",MB_OK);
	Inline_InstallHook();
	MessageBoxA(NULL,"After  Inline Hook","Test",MB_OK);
	Inline_UnInstallHook();
	MessageBoxA(NULL,"After  Inline Hook Unhooked","Test",MB_OK);
	return 0;
}

//************************************
// Method:    FakeMessageBox
// FullName:  FakeMessageBox
// Purpose:   取代原始MessageBoxA的功能,HOOK后所有对MessageBoxA的调用将实际调用本函数
// Author:    achillis
// Returns:   int WINAPI
// Parameter: HWND hWnd
// Parameter: LPCTSTR lpText
// Parameter: LPCTSTR lpCaption
// Parameter: UINT uType
//************************************
//注意函数的定义和原始函数一定要一样，尤其是调用约定，否则函数返回后将出错
int WINAPI My_MessageBoxA(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
{
	//在这里，你可以对原始参数进行任意操作
	int ret;
	char newText[1024]={0};
	char newCaption[256]="pediy.com";
	printf("有人调用MessageBox!\n");
	//在调用原函数之前，可以对IN(输入类)参数进行干涉
	lstrcpy(newText,lpText);//为防止原函数提供的缓冲区不够，这里复制到我们自己的一个缓冲区中再进行操作
	lstrcat(newText,"\n\tMessageBox Hacked by pediy.com!");//篡改消息框内容
	uType|=MB_ICONERROR;//增加一个错误图标
	ret = TrampolineMessageBox(hWnd,newText,newCaption,uType);//调用原MessageBox，并保存返回值
	//调用原函数之后，可以继续对OUT(输出类)参数进行干涉,比如网络函数的recv，可以干涉返回的内容
	return ret;//这里你还可以干涉原始函数的返回值
}

BOOL Inline_InstallHook()
{
	//准备Hook
	ZeroMemory(&MsgBoxHookData,sizeof(HOOK_DATA));
	strcpy(MsgBoxHookData.szApiName , "MessageBoxA");
	strcpy(MsgBoxHookData.szModuleName , "user32.dll");
	MsgBoxHookData.HookPoint = (ULONG_PTR)GetAddress(MsgBoxHookData.szModuleName,MsgBoxHookData.szApiName);//HOOK的地址
	MsgBoxHookData.pfnTrampolineFun = (ULONG_PTR)TrampolineMessageBox;//调用原始函数的通道
	MsgBoxHookData.pfnDetourFun = (ULONG_PTR)My_MessageBoxA;//DetourFun
	MsgBoxHookData.HookCodeLen = HOOKCODELEN;
	return InstallCodeHook(&MsgBoxHookData);
}


BOOL Inline_UnInstallHook()
{
	return UninstallCodeHook(&MsgBoxHookData);
}
/*
VirtualAllocEx的代码开头:
7C809B12 >  6A 10              push 10
7C809B14    68 609B807C        push kernel32.7C809B60
7C809B19    E8 B889FFFF        call kernel32.7C8024D6
*/
//当需要调用原始的MessageBox时，直接调用此函数即可，参数完全相同
__declspec( naked )
int WINAPI TrampolineMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
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
		jmp MsgBoxHookData.JmpBackAddr //跳到Hook代码之后的地方，绕过自己安装的HOOK
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
			/*
			77D507E5   >-/E9 66086B88      jmp InlineHo.00401050
			77D507EA > $^\EB F9            jmp short USER32.77D507E5
			*/
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
		|| (pfnHead[0] == 0x6A && pfnHead[5] == 0x68)) //push sehhandler
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
