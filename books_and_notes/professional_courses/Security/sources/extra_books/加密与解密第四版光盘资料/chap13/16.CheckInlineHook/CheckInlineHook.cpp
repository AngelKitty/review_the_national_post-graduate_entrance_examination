/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// CheckInlineHook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "PELoader.h"

//定义如下结构，保存一次InlineHook所需要的信息
typedef struct _HOOK_DATA{
	char szApiName[128];	//待Hook的API名字
	char szModuleName[64];	//待Hook的API所属模块的名字
	int  HookCodeLen;		//Hook长度
	BYTE oldEntry[16];		//保存Hook位置的原始指令
	BYTE newEntry[16];		//保存要写入Hook位置的新指令
	ULONG HookPoint;		//待HOOK的位置
	PVOID pfnOriginalFun;	//调用原始函数的通道
	PVOID pfnFakeFun;		//HOOK过滤函数
}HOOK_DATA,*PHOOK_DATA;

#define HOOKLEN (5)	//要改写的指令的长度
HOOK_DATA MsgBoxHookData;

LPVOID GetAddress(char *,char *);
void makehookentry(PVOID HookPoint);
int WINAPI My_MessageBoxA(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
int WINAPI OriginalMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
BOOL Inline_InstallHook(void);
BOOL Inline_UnInstallHook();
BOOL InstallCodeHook(PHOOK_DATA pHookData);
BOOL UninstallCodeHook(PHOOK_DATA pHookData);

VOID CheckModuleInlineHook(char *szModName);
SIZE_T CompareMemory(VOID  *Source1,VOID  *Source2,SIZE_T  Length);

int main(int argc, char* argv[])
{
	Inline_InstallHook();
	MessageBoxA(NULL,"After  Inline Hook","Test",MB_OK);
	//检测user32.dll模块的Inline Hook
	CheckModuleInlineHook("user32.dll");
	return 0;
}

VOID CheckModuleInlineHook(char *szModName)
{
	PELoader Ldr;
	char szModPath[MAX_PATH] = {0};
	HMODULE hTargetMod = GetModuleHandle(szModName);

	GetModuleFileName(hTargetMod,szModPath,MAX_PATH);
	PBYTE pNewUser32 = Ldr.LoadPE(szModPath,TRUE,(DWORD)hTargetMod,TRUE);
	printf("[*] pNewUser32 = 0x%08X\n",pNewUser32);

	PIMAGE_SECTION_HEADER pCodeSec = Ldr.m_pSecHeader ;
	WORD i = 0 ;

	//查找.text代码段的位置
	for (i=0;i<Ldr.m_SectionCnt;i++)
	{
		if (strcmp((char*)pCodeSec->Name,".text") == 0)
		{
			break;
		}
	}

	printf("[*] CodeSection = 0x%08X\n",pCodeSec);
	PBYTE pOriginalCodeStart = Ldr.m_hModule + pCodeSec->VirtualAddress;
	PBYTE pCurrentCodeStart = (PBYTE)hTargetMod + pCodeSec->VirtualAddress ;
	DWORD dwCodeSize = pCodeSec->SizeOfRawData;
	printf("[*] pOriginalCode = 0x%08X  pCurCode = 0x%08X  Size = 0x%X\n",pOriginalCodeStart,pCurrentCodeStart,dwCodeSize);
	
	DWORD dwScanOffset = 0;
	DWORD dwDifCnt = 0 ;
	PBYTE pOrgCode,pCurCode;
	DWORD EqualSize = 0 ;
	BYTE  OrgCode[1024]={0};
	BYTE  CurCode[1024]={0};
	DWORD m = 0 ,n = 0;
	DWORD JmpAddress = 0 ;
	pOrgCode = pOriginalCodeStart;
	pCurCode = pCurrentCodeStart ;

	printf("[*] 开始检测Inline Hook.\n");
	while (dwScanOffset < dwCodeSize)
	{
		EqualSize = CompareMemory(pOrgCode,pCurCode,dwCodeSize - dwScanOffset);
		dwScanOffset += EqualSize;
		if (dwScanOffset == dwCodeSize)
		{
			break;
		}
		pOrgCode += EqualSize;
		pCurCode += EqualSize;
		printf("[%2d]Inline Hook Found at : 0x%08X\n",dwDifCnt++,pCurCode);

		//跳过不同的地方并保存这段指令
		m = 0 ;
		while (*pOrgCode != *pCurCode)
		{
			OrgCode[m] = *pOrgCode++;
			CurCode[m] = *pCurCode++;
			m++;
		}

		//显示不一样的代码
		printf("原始代码 : ");
		for (n=0;n<m;n++)
		{
			printf("%02X ",OrgCode[n]);
		}
		printf("\n");
		printf("当前代码 : ");
		for (n=0;n<m;n++)
		{
			printf("%02X ",CurCode[n]);
		}

		if (CurCode[0] == 0xE9)
		{
			//计算jmp地址
			JmpAddress = (DWORD)pCurCode + *(DWORD*)(CurCode + 1) + 5 ;
			printf(" => Jmp %08X",JmpAddress);
		}
		printf("\n");
		printf("====================================\n");
		
		dwScanOffset += m;
	}

	printf("[*] Inline Hook检测完毕.\n");
}


//比较两段内存，返回其相等的字节数，作用与RtlCompareMemory相同
SIZE_T CompareMemory(VOID  *Source1,VOID  *Source2,SIZE_T  Length)
{
	SIZE_T Offset = 0 ;
	PBYTE pS1 = (PBYTE)Source1;
	PBYTE pS2 = (PBYTE)Source2;

	while (Offset < Length
		&& *pS1++ == *pS2++)
	{
		Offset++;
	}

	return Offset;
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
	ret = OriginalMessageBox(hWnd,newText,newCaption,uType);//调用原MessageBox，并保存返回值
	//调用原函数之后，可以继续对OUT(输出类)参数进行干涉,比如网络函数的recv，可以干涉返回的内容
	return ret;//这里你还可以干涉原始函数的返回值
}

BOOL Inline_InstallHook()
{
	//准备Hook
	ZeroMemory(&MsgBoxHookData,sizeof(HOOK_DATA));
	strcpy(MsgBoxHookData.szApiName , "MessageBoxA");
	strcpy(MsgBoxHookData.szModuleName , "user32.dll");
	MsgBoxHookData.HookPoint = (ULONG)GetAddress(MsgBoxHookData.szModuleName,MsgBoxHookData.szApiName);//HOOK的地址
	MsgBoxHookData.pfnOriginalFun = (PVOID)OriginalMessageBox;//调用原始函数的通道
	MsgBoxHookData.pfnFakeFun = (PVOID)My_MessageBoxA;//Fake
	MsgBoxHookData.HookCodeLen = 5;
	return InstallCodeHook(&MsgBoxHookData);
}


BOOL Inline_UnInstallHook()
{
	return UninstallCodeHook(&MsgBoxHookData);
}
/*
MessageBoxA的代码开头:
77D5050B >  8BFF                   mov edi,edi
77D5050D    55                     push ebp
77D5050E    8BEC                   mov ebp,esp
77D50510    833D 1C04D777 00       cmp dword ptr ds:[gfEMIEnable],0
*/
//当需要调用原始的MessageBox时，直接调用此函数即可，参数完全相同
__declspec( naked )
int WINAPI OriginalMessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType)
{
	_asm
	{
		//由于我们写入的Jmp指令破坏了原来的前3条指令,因此在这里执行原函数的前3条指令
		mov edi,edi  //这一句其实可以不要
		push ebp
		mov ebp,esp
		mov eax,MsgBoxHookData.HookPoint //取原始地址
		add eax,MsgBoxHookData.HookCodeLen //加上被Hook代码的长度
		jmp eax //跳到Hook代码之后的地方，绕过自己安装的HOOK
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
	BYTE Jmp=0xE9;
	if (pHookData==NULL
		|| pHookData->pfnFakeFun==NULL
		|| pHookData->HookPoint==NULL)
	{
		return;
	}

	pHookData->newEntry[0] = 0xE9; //Jmp 
	//计算跳转偏移并写入
	*(ULONG*)(pHookData->newEntry+1) = (ULONG)pHookData->pfnFakeFun - (ULONG)pHookData->HookPoint - 5;//0xE9 式jmp的计算
	
	
}

BOOL InstallCodeHook(PHOOK_DATA pHookData)
{
	DWORD dwBytesReturned=0;
	DWORD dwOldProtect;
	HANDLE hProcess=GetCurrentProcess();
	MEMORY_BASIC_INFORMATION MBI={0};
	BOOL bResult=FALSE;
	if (pHookData==NULL
		|| pHookData->HookPoint==0
		|| pHookData->pfnFakeFun==NULL
		|| pHookData->pfnOriginalFun==NULL)
	{
		return FALSE;
	}
	LPVOID OriginalAddr=(LPVOID)pHookData->HookPoint;
	printf("Address To HOOK=0x%08X\n",OriginalAddr);
	InitHookEntry(pHookData);//填充Inline Hook代码
	if(ReadProcessMemory(hProcess,OriginalAddr,pHookData->oldEntry,pHookData->HookCodeLen,&dwBytesReturned))
	{
		if (VirtualQueryEx(hProcess,OriginalAddr,&MBI,sizeof(MEMORY_BASIC_INFORMATION))
			&& VirtualProtectEx(hProcess,MBI.BaseAddress,pHookData->HookCodeLen,PAGE_EXECUTE_READWRITE,&dwOldProtect))
		{
			if (WriteProcessMemory(hProcess,OriginalAddr,pHookData->newEntry,pHookData->HookCodeLen,&dwBytesReturned))
			{
				printf("Install Hook write oK! WrittenCnt=%d\n",dwBytesReturned);
				bResult=TRUE;
			}
			VirtualProtectEx(hProcess,MBI.BaseAddress,pHookData->HookCodeLen,dwOldProtect,&dwOldProtect);
		}
	}
	return bResult;
}

BOOL UninstallCodeHook(PHOOK_DATA HookData)
{
	DWORD dwBytesReturned=0;
	DWORD dwOldProtect;
	HANDLE hProcess=GetCurrentProcess();
	BOOL bResult=FALSE;
	MEMORY_BASIC_INFORMATION MBI={0};
	LPVOID OriginalAddr;
	if (HookData==NULL
		|| HookData->HookPoint==0
		|| HookData->oldEntry[0]==0)
	{
		return FALSE;
	}
	OriginalAddr=(LPVOID)HookData->HookPoint;
	if (VirtualQueryEx(hProcess,OriginalAddr,&MBI,sizeof(MEMORY_BASIC_INFORMATION))
		&& VirtualProtectEx(hProcess,MBI.BaseAddress,HookData->HookCodeLen,PAGE_EXECUTE_READWRITE,&dwOldProtect))
	{
		if (WriteProcessMemory(hProcess,OriginalAddr,HookData->oldEntry,HookData->HookCodeLen,&dwBytesReturned))
		{
			bResult=TRUE;
		}
		VirtualProtectEx(hProcess,MBI.BaseAddress,HookData->HookCodeLen,dwOldProtect,&dwOldProtect);
	}
	return bResult;
}