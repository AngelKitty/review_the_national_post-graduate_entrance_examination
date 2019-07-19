/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// VEHHook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

BOOL SetBreakPoint(PVOID pFuncAddr);
BOOL ClearBreakPoint(PVOID pFuncAddr);
BOOL InstallVEHHook(PVECTORED_EXCEPTION_HANDLER Handler);
VOID UnInstallVEHHook();

typedef int 
(WINAPI *PFN_MessageBox)(
	HWND hWnd,          // handle of owner window
	LPCTSTR lpText,     // address of text in message box
	LPCTSTR lpCaption,  // address of title of message box
	UINT uType          // style of message box
	);

int WINAPI My_MessageBox(
	HWND hWnd,          // handle of owner window
	LPCTSTR lpText,     // address of text in message box
	LPCTSTR lpCaption,  // address of title of message box
	UINT uType          // style of message box
	);

LONG WINAPI VectoredHandler1(struct _EXCEPTION_POINTERS *ExceptionInfo);
LONG WINAPI VectoredHandler2(struct _EXCEPTION_POINTERS *ExceptionInfo);
LONG WINAPI VectoredHandler3(struct _EXCEPTION_POINTERS *ExceptionInfo);
VOID ShowMsgBox(LPCTSTR lpMsg);
ULONG_PTR InitTrampolineFun();

PFN_MessageBox g_OriginalMessageBoxA;
PVOID g_AddrofMessageBoxA = 0 ;
PVOID g_hVector;
BYTE  g_OldCode[16]={0};

int main(int argc, char* argv[])
{
	HMODULE hUser32 = LoadLibrary("user32.dll");
	g_AddrofMessageBoxA =  (PVOID)GetProcAddress(hUser32,"MessageBoxA");
	printf("Address of MessageBoxA = 0x%p\n",g_AddrofMessageBoxA);
	g_OriginalMessageBoxA = (PFN_MessageBox)InitTrampolineFun();  //跳过开头的Hook
	
	printf("Addr of VectoredHandler1 = 0x%p\n",VectoredHandler1);
	printf("Addr of VectoredHandler2 = 0x%p\n",VectoredHandler2);
	printf("Addr of VectoredHandler3 = 0x%p\n",VectoredHandler3);

	//选择安装一个进行测试
	InstallVEHHook(VectoredHandler3);

	//设置断点
	SetBreakPoint(g_AddrofMessageBoxA);

	//call
	ShowMsgBox("VEH Hook Test.");

	printf("All Finished!\n");
	ClearBreakPoint(g_AddrofMessageBoxA);
	UnInstallVEHHook();
	

	ShowMsgBox("Hook Cleared");
	return 0;
}

VOID ShowMsgBox(LPCTSTR lpMsg)
{
	MessageBoxA(NULL,lpMsg,"Test",MB_OK);
}

ULONG_PTR InitTrampolineFun()
{
	ULONG_PTR uResult = 0 ;
	PBYTE pFun = NULL;

#ifdef _WIN64
	//x64需要申请shellcode
	/*
	USER32!MessageBoxA:
	00000000`779412b8 4883ec38        sub     rsp,38h
	00000000`779412bc 4533db          xor     r11d,r11d
	00000000`779412bf 44391d760e0200  cmp     dword ptr [USER32!gapfnScSendMessage+0x927c (00000000`7796213c)],r11d
	*/
	pFun = (PBYTE)VirtualAlloc(NULL,128,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	uResult = (ULONG_PTR)pFun;
	
	memset(pFun,0,128);
	memcpy(pFun,(PVOID)g_AddrofMessageBoxA,4); //拷贝第一条指令，4字节,推荐使用反汇编引擎来实际计算
	pFun += 4 ; //下一条指令构造为jmp [xxxxxx]
	pFun[0] = 0xFF;
	pFun[1] = 0x25;
	*(ULONG_PTR*)(pFun + 6) = (ULONG_PTR)g_AddrofMessageBoxA + 4 ; //跳回到原函数加4的地方
#else
	//x86,第一条指令是mov edi,edi,直接跳过即可
	uResult = (ULONG_PTR)g_AddrofMessageBoxA + 2;
#endif

	return uResult;
}


//处理方式，修改参数并返回原函数继续执行
LONG WINAPI
VectoredHandler1(
	struct _EXCEPTION_POINTERS *ExceptionInfo
	)
{
	char *szNewMsg =  "[VectoredHandler1] Hacked by pediy.com";
	LONG lResult = EXCEPTION_CONTINUE_SEARCH ;
	PEXCEPTION_RECORD pExceptionRecord;  
	PCONTEXT pContextRecord;
	int ret = 0 ;
	pExceptionRecord = ExceptionInfo->ExceptionRecord ;
	pContextRecord = ExceptionInfo->ContextRecord ;
	ULONG_PTR* uESP = 0 ;
	printf("Exception Address = %p\n",pExceptionRecord->ExceptionAddress);
	if (pExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT
		&& pExceptionRecord->ExceptionAddress == g_AddrofMessageBoxA)
	{
#ifdef _WIN64
		//x64上前四个参数依次为RCX,RDX,R8,R9
		//修改第二个参数，即LpMsg
		printf("lpText = 0x%p   %s\n",pContextRecord->Rdx,(char*)pContextRecord->Rdx);
		pContextRecord->Rdx = (ULONG_PTR)szNewMsg;
		pContextRecord->Rip = (ULONG_PTR)g_OriginalMessageBoxA ; //跳到Trampoline继续执行
#else
		/*
		0012FF70   0040105A   /CALL 到 MessageBoxA 来自 VEHHook.00401054
		0012FF74   00000000   |hOwner = NULL
		0012FF78   00407030   |Text = "VEH Hook"
		0012FF7C   0040703C   |Title = "Test"
		0012FF80   00000000   \Style = MB_OK|MB_APPLMODAL
		0012FF84   00401225   返回到 VEHHook.<ModuleEntryPoint>+0B4 来自 VEHHook.00401000
		*/
		printf("ESP = 0x%p\n",pContextRecord->Esp) ;
		uESP = (ULONG_PTR*)pContextRecord->Esp ; //取中断时的ESP
		uESP[2] = (ULONG_PTR)szNewMsg; //修改栈中的参数
		pContextRecord->Eip = (ULONG_PTR)g_OriginalMessageBoxA ; //跳过函数开头
#endif
		
		lResult = EXCEPTION_CONTINUE_EXECUTION ;
	}
    return lResult;
}

//处理方式：直接调用原函数并替原函数返回
LONG WINAPI
VectoredHandler2(
	struct _EXCEPTION_POINTERS *ExceptionInfo
	)
{
	char *szNewMsg =  "[VectoredHandler2] Hacked by pediy.com";
	LONG lResult = EXCEPTION_CONTINUE_SEARCH ;
	PEXCEPTION_RECORD pExceptionRecord;  
	PCONTEXT pContextRecord;
	int ret = 0 ;
	pExceptionRecord = ExceptionInfo->ExceptionRecord ;
	pContextRecord = ExceptionInfo->ContextRecord ;
	ULONG_PTR* uESP = 0 ;
	if (pExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT
		&& pExceptionRecord->ExceptionAddress == g_AddrofMessageBoxA)
	{
		
#ifdef _WIN64
		//x64上前四个参数依次为RCX,RDX,R8,R9
		printf("RSP = 0x%p\n",pContextRecord->Rsp) ;
		uESP = (ULONG_PTR*)pContextRecord->Rsp ;
		printf("Return Address = 0x%p\n",uESP[0]);
		ret = g_OriginalMessageBoxA((HWND)pContextRecord->Rcx,szNewMsg,(LPCTSTR)pContextRecord->R8,(int)pContextRecord->R9);
		printf("ret = %d\n",ret);
		//修正RSP
		pContextRecord->Rsp += sizeof(ULONG_PTR);//参数在寄存器中，栈中无参数，仅需跳过返回地址
		//直接返回到调用者处
		pContextRecord->Rip = uESP[0] ;//设置EIP为返回地址
#else
		/*
		0012FF70   0040105A   /CALL 到 MessageBoxA 来自 VEHHook.00401054
		0012FF74   00000000   |hOwner = NULL
		0012FF78   00407030   |Text = "VEH Hook"
		0012FF7C   0040703C   |Title = "Test"
		0012FF80   00000000   \Style = MB_OK|MB_APPLMODAL
		0012FF84   00401225   返回到 VEHHook.<ModuleEntryPoint>+0B4 来自 VEHHook.00401000
		*/
		printf("ESP = 0x%p\n",pContextRecord->Esp) ;
		uESP = (ULONG_PTR*)pContextRecord->Esp ;
		ret = g_OriginalMessageBoxA((HWND)uESP[1],szNewMsg,(LPCTSTR)uESP[3],(int)uESP[4]);
		printf("ret = %d\n",ret);

		//直接返回到调用者处
		pContextRecord->Eip = uESP[0] ;//设置EIP为返回地址
		pContextRecord->Esp += (4 + 1)*sizeof(ULONG_PTR); //4为参数个数,1为返回地址
#endif
		
		lResult = EXCEPTION_CONTINUE_EXECUTION ;
	}
    return lResult;
}

//处理方式：直接返回,相当于过滤掉
LONG WINAPI
VectoredHandler3(
	struct _EXCEPTION_POINTERS *ExceptionInfo
	)
{
	LONG lResult = EXCEPTION_CONTINUE_SEARCH ;
	PEXCEPTION_RECORD pExceptionRecord = ExceptionInfo->ExceptionRecord ;
	PCONTEXT pContextRecord = ExceptionInfo->ContextRecord ;
	ULONG_PTR* uESP = 0 ;
	if (pExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT
		&& pExceptionRecord->ExceptionAddress == g_AddrofMessageBoxA)
	{
		
		/*
		0012FF70   0040105A   /CALL 到 MessageBoxA 来自 VEHHook.00401054
		0012FF74   00000000   |hOwner = NULL
		0012FF78   00407030   |Text = "VEH Hook"
		0012FF7C   0040703C   |Title = "Test"
		0012FF80   00000000   \Style = MB_OK|MB_APPLMODAL
		0012FF84   00401225   返回到 VEHHook.<ModuleEntryPoint>+0B4 来自 VEHHook.00401000
		*/
		

		//直接返回到调用者处
#ifdef _WIN64
		printf("RSP = 0x%p\n",pContextRecord->Rsp) ;
		uESP = (ULONG_PTR*)pContextRecord->Rsp ;
		pContextRecord->Rip = uESP[0] ;//设置EIP为返回地址
		pContextRecord->Rsp += sizeof(ULONG_PTR); //将压入栈内的参数和返回地址清理掉,4为参数个数,1为返回地址
#else
		printf("ESP = 0x%X\n",pContextRecord->Esp) ;
		uESP = (ULONG_PTR*)pContextRecord->Esp ;
		pContextRecord->Eip = uESP[0] ;//设置EIP为返回地址
		pContextRecord->Esp += (4 + 1)*sizeof(ULONG_PTR); //将压入栈内的参数和返回地址清理掉,4为参数个数,1为返回地址
#endif
		
		lResult = EXCEPTION_CONTINUE_EXECUTION ;
	}
    return lResult;
}

BOOL InstallVEHHook(PVECTORED_EXCEPTION_HANDLER Handler)
{
	printf("Current Handler Address = 0x%p\n",Handler);
	g_hVector = AddVectoredExceptionHandler(1,Handler);
	return g_hVector != NULL ;
}

VOID UnInstallVEHHook()
{
	RemoveVectoredExceptionHandler(g_hVector);
}

/*
0:000> u user32!messageboxA
USER32!MessageBoxA:
77d507ea 8bff            mov     edi,edi
77d507ec 55              push    ebp
77d507ed 8bec            mov     ebp,esp
*/
BOOL SetBreakPoint(PVOID pFuncAddr)
{
	DWORD dwCnt = 0 ;
	BYTE *pTarget = (BYTE*)pFuncAddr;
	
	
	g_OldCode[0] = *pTarget;
	printf("Original Fun Head Code = 0x%02X\n",g_OldCode[0]);
	//修改内存页的属性
	DWORD dwOLD;
	MEMORY_BASIC_INFORMATION  mbi;
	VirtualQuery(pTarget,&mbi,sizeof(mbi));
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&dwOLD);
	
	//写入int3
	*pTarget  = 0xCC ;

	//恢复内存页的属性
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOLD,0);
	return TRUE;
}

BOOL ClearBreakPoint(PVOID pFuncAddr)
{
	BYTE *pTarget = (BYTE*)pFuncAddr;
	//修改内存页的属性
	DWORD dwOLD;
	MEMORY_BASIC_INFORMATION  mbi;
	VirtualQuery(pTarget,&mbi,sizeof(mbi));
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&dwOLD);
	
	*pTarget  = g_OldCode[0] ;
	
	//恢复内存页的属性
	VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOLD,0);
	return TRUE;
}

int WINAPI My_MessageBox(
	HWND hWnd,          // handle of owner window
	LPCTSTR lpText,     // address of text in message box
	LPCTSTR lpCaption,  // address of title of message box
	UINT uType          // style of message box
	)
{	
	char newMsg[400];
	char newCation[]="标题被我改了!";
	int result;
	if (lpText)
	{
		ZeroMemory(newMsg,400);
		lstrcpy(newMsg,lpText);
		lstrcat(newMsg,"\n\tMessage Box hacked by pediy.com");
	}
	printf("有人调用MessageBox...\n");
	result = g_OriginalMessageBoxA(hWnd,newMsg,newCation,uType);
	return result;
	
}
