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
//本程序中写内存不再使用繁琐的VirtualProtect函数，直接使用WriteProcessMemory

#include <windows.h>
#include <stdio.h>
#include <CONIO.H>

//定义如下结构，保存一次InlineHook所需要的信息
typedef struct _HOOK_DATA{
	char szApiName[128];	//待Hook的API名字
	char szModuleName[64];	//待Hook的API所属模块的名字
	int  HookCodeLen;		//Hook长度
	BYTE oldEntry[16];		//保存Hook位置的原始指令
	BYTE newEntry[16];		//保存要写入Hook位置的新指令
	ULONG_PTR HookPoint;		//待HOOK的位置
	ULONG_PTR JmpBackAddr;		//回跳到原函数中的位置
	ULONG_PTR pfnTrampolineFun;	//调用原始函数的通道
	ULONG_PTR pfnDetourFun;		//HOOK过滤函数
}HOOK_DATA,*PHOOK_DATA;

HOOK_DATA MsgBoxHookData;

typedef int 
(WINAPI *PFN_MessageBoxA)(
	HWND hWnd,
	LPCTSTR lpText,
	LPCTSTR lpCaption,
	UINT uType
	);

ULONG_PTR SkipJmpAddress(ULONG_PTR uAddress);
LPVOID GetAddress(char *,char *);
void makehookentry(PVOID HookPoint);
VOID InitTrampoline(PHOOK_DATA pHookData);
int WINAPI My_MessageBoxA(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption,UINT uType);
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
	PFN_MessageBoxA OriginalMessageBox = (PFN_MessageBoxA)MsgBoxHookData.pfnTrampolineFun;
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
	MsgBoxHookData.HookCodeLen = 14;
	MsgBoxHookData.HookPoint = (ULONG_PTR)GetAddress(MsgBoxHookData.szModuleName,MsgBoxHookData.szApiName);//HOOK的地址
	//MsgBoxHookData.pfnOriginalFun = (PVOID)OriginalMessageBox;//调用原始函数的通道
	//x64下不能内联汇编了，所以申请一块内存用做TrampolineFun的shellcode
	MsgBoxHookData.pfnTrampolineFun = (ULONG_PTR)VirtualAlloc(NULL,128,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	MsgBoxHookData.pfnDetourFun = (ULONG_PTR)My_MessageBoxA;//Fake
	return InstallCodeHook(&MsgBoxHookData);
}


BOOL Inline_UnInstallHook()
{
	return UninstallCodeHook(&MsgBoxHookData);
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
	使用的是jmp [xxxx]
*/
void InitHookEntry(PHOOK_DATA pHookData)
{
	if (pHookData==NULL
		|| pHookData->pfnDetourFun==NULL
		|| pHookData->HookPoint==NULL)
	{
		return;
	}
	
	memset(pHookData->newEntry,0,14);
	pHookData->newEntry[0] = 0xFF; //Jmp
	pHookData->newEntry[1] = 0x25; //Jmp
	// ff2500000000    jmp     qword ptr [00000000`77a78706]
	*(ULONG_PTR*)(pHookData->newEntry+6) = (ULONG_PTR)pHookData->pfnDetourFun ;
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
USER32!MessageBoxA:
00000000`779412b8 4883ec38        sub     rsp,38h
00000000`779412bc 4533db          xor     r11d,r11d
00000000`779412bf 44391d760e0200  cmp     dword ptr [USER32!gapfnScSendMessage+0x927c (00000000`7796213c)],r11d
*/
VOID InitTrampoline(PHOOK_DATA pHookData)
{
	//保存前14字节
	
	PBYTE pFun = (PBYTE)pHookData->pfnTrampolineFun ;
	memcpy(pFun,(PVOID)pHookData->HookPoint,14);

	//由于第三行指令中有重定位数据，所以这里需要修复一下
	//更好的办法是使用反汇编引擎来判断是否有重定位数据
	ULONG DataOffset = 0 ;
	ULONG_PTR pData = (ULONG_PTR)pHookData->HookPoint + 7 + 7  + *(ULONG*)(pHookData->HookPoint + 10) ;
	printf("pData = 0x%p\n",pData);
	DataOffset = (ULONG)(pData - ( (ULONG_PTR)pFun + 14 )) ;
	*(ULONG*)(pFun + 10) = DataOffset;

	//在后面添加一个跳转指令
	pFun += 14; //跳过前三行指令
	pFun[0] = 0xFF;
	pFun[1] = 0x25;
	*(ULONG_PTR*)(pFun + 6) = pHookData->JmpBackAddr ;

}


BOOL InstallCodeHook(PHOOK_DATA pHookData)
{
	SIZE_T dwBytesReturned=0;
	HANDLE hProcess=GetCurrentProcess();
	BOOL bResult=FALSE;
	if (pHookData==NULL
		|| pHookData->HookPoint==0
		|| pHookData->pfnDetourFun==NULL
		|| pHookData->pfnTrampolineFun==NULL)
	{
		return FALSE;
	}
	pHookData->HookPoint = SkipJmpAddress(pHookData->HookPoint); //如果函数开头是跳转，那么将其跳过
	pHookData->JmpBackAddr = pHookData->HookPoint + pHookData->HookCodeLen ;
	LPVOID OriginalAddr = (LPVOID)pHookData->HookPoint ;
	printf("Address To HOOK=0x%p\n",OriginalAddr);
	InitHookEntry(pHookData);//填充Inline Hook代码
	InitTrampoline(pHookData);//构造Trampoline
	if(ReadProcessMemory(hProcess,OriginalAddr,pHookData->oldEntry,pHookData->HookCodeLen,&dwBytesReturned))
	{
		if (WriteProcessMemory(hProcess,OriginalAddr,pHookData->newEntry,pHookData->HookCodeLen,&dwBytesReturned))
		{
			printf("Install Hook write OK! WrittenCnt=%d\n",dwBytesReturned);
			bResult=TRUE;
		}
	}
	return bResult;
}

BOOL UninstallCodeHook(PHOOK_DATA HookData)
{
	SIZE_T dwBytesReturned=0;
	HANDLE hProcess=GetCurrentProcess();
	BOOL bResult=FALSE;
	LPVOID OriginalAddr;
	if (HookData==NULL
		|| HookData->HookPoint==0
		|| HookData->oldEntry[0]==0)
	{
		return FALSE;
	}
	OriginalAddr=(LPVOID)HookData->HookPoint;
	bResult = WriteProcessMemory(hProcess,OriginalAddr,HookData->oldEntry,HookData->HookCodeLen,&dwBytesReturned) ;
	return bResult;
}