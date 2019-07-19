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
//程序功能:为user32.dll安装EAT Hook，目标函数是MessageBoxA

#include <windows.h>
#include <stdio.h>
#include <imagehlp.h>

#pragma comment(lib,"imagehlp.lib")

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

BOOL InstallModuleEATHook(
	HMODULE hModToHook,// IN
	char *szFuncName,// IN
	PVOID ProxyFunc,// IN
	PULONG_PTR *pAddrPointer,//OUT
	ULONG_PTR *pOriginalFuncAddr//OUT
	);

int main(int argc, char* argv[])
{
	HMODULE hUser32 = LoadLibrary("user32.dll");
	PULONG_PTR pEATPointer;
	ULONG_PTR uOldRVA;
	InstallModuleEATHook(hUser32,"MessageBoxA",My_MessageBox,&pEATPointer,&uOldRVA);
	printf("pEATPointer = 0x%X OldRVA = 0x%X\n",pEATPointer,uOldRVA);
	

	//Test
	printf("Now Test the EAT Hook.\n");
	PFN_MessageBox pMsgBox = (PFN_MessageBox)GetProcAddress(GetModuleHandle("user32.dll"),"MessageBoxA");
	printf("dwAddr = 0x%p\n",pMsgBox);
	pMsgBox(NULL,"EAT Hook","Test",MB_OK);
	

	//加载一个弹MessageBox的Dll做测试
#ifdef _WIN64
	LoadLibrary("../Dll/MsgDll64.dll");
#else
	LoadLibrary("../Dll/MsgDll.dll");
#endif
	return 0;
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
	result=MessageBoxA(hWnd,newMsg,newCation,uType);
	return result;
	
}

BOOL InstallModuleEATHook(
	HMODULE hModToHook,// IN
	char *szFuncName,// IN
	PVOID ProxyFunc,// IN
	PULONG_PTR *pAddrPointer,//OUT
	ULONG_PTR *pOriginalFuncAddr//OUT
	)
{
	PIMAGE_EXPORT_DIRECTORY	  pExportDir;
	ULONG ulSize;
	ULONG_PTR TargetFunAddr;
	BOOL result = FALSE ;
	ULONG nFuncCnt = 0 ;
	ULONG i = 0 ;
	ULONG_PTR TargetFunRVA = 0 ;
	ULONG *funrav = NULL ;
	
	TargetFunAddr = (ULONG_PTR)GetProcAddress(hModToHook,szFuncName);
	TargetFunRVA = (ULONG)(TargetFunAddr - (ULONG_PTR)hModToHook); 
	printf("[*]Address of %s:0x%p  RVA = 0x%X\n",szFuncName,TargetFunAddr,TargetFunRVA);
	printf("[*]Module To Hook at Base:0x%p\n",hModToHook);
	pExportDir = (PIMAGE_EXPORT_DIRECTORY)ImageDirectoryEntryToData(hModToHook, TRUE,IMAGE_DIRECTORY_ENTRY_EXPORT, &ulSize);
	printf("[*]Find ExportTable,Address:0x%p\n",pExportDir);
	nFuncCnt = pExportDir->NumberOfFunctions ;
	funrav=(ULONG*)((BYTE*)hModToHook+pExportDir->AddressOfFunctions);
	for (i=0;i<nFuncCnt;i++)
	{
		if (funrav[i] == TargetFunRVA)
		{
			printf("[*]Find target address!\n");
			//修改内存页的属性
			DWORD dwOLD;
			MEMORY_BASIC_INFORMATION  mbi;
			VirtualQuery(&funrav[i],&mbi,sizeof(mbi));
			VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&dwOLD);
			//保存原始数据
			if (pAddrPointer)
			{
				*pAddrPointer = (PULONG_PTR)&funrav[i] ;
			}
			if (pOriginalFuncAddr)
			{
				*pOriginalFuncAddr = funrav[i] ;
			}
			//修改地址
			funrav[i] = (ULONG)((ULONG_PTR)ProxyFunc - (ULONG_PTR)hModToHook);
			result = TRUE ;
			//恢复内存页的属性
			VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOLD,0);
			printf("[*]Hook ok.\n");
			break;
		}
	}
	return result;
}