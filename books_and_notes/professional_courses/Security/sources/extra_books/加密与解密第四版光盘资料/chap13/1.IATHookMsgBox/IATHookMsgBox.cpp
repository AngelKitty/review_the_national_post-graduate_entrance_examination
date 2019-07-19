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
//程序功能:为本进程的exe模块安装IAT Hook，目标函数是MessageBoxA

#include <windows.h>
#include <stdio.h>
#include <imagehlp.h>
#pragma comment(lib,"imagehlp.lib")


//以MessageBoxA的原型定义一个函数指针类型
typedef int 
(WINAPI *PFN_MessageBoxA)(
	HWND hWnd,          // handle of owner window
	LPCTSTR lpText,     // address of text in message box
	LPCTSTR lpCaption,  // address of title of message box
	UINT uType          // style of message box
	);

//以MessageBoxA的原型定义一个函数来替代原始的MessageBoxA
int WINAPI My_MessageBoxA(
	HWND hWnd,          // handle of owner window
	LPCTSTR lpText,     // address of text in message box
	LPCTSTR lpCaption,  // address of title of message box
	UINT uType          // style of message box
	);

//存在以下关系
//*(*pThunkPointer) == *pOriginalFuncAddr ;
BOOL InstallModuleIATHook(
	HMODULE hModToHook,
	char *szModuleName, 
	char *szFuncName,
	PVOID ProxyFunc,
	PULONG_PTR *pThunkPointer,
	ULONG_PTR *pOriginalFuncAddr
	);

VOID ShowMsgBox(char *szMsg);
BOOL IAT_InstallHook();
VOID IAT_UnInstallHook();
BOOL IsWow64();
//保存原始MessageBoxA的地址
PFN_MessageBoxA OldMessageBox=NULL;
//指向IAT中pThunk的地址
PULONG_PTR g_PointerToIATThunk = NULL;

int main(int argc, char *argv[ ])
{
	BOOL bIsWow64 = IsWow64();
	printf("IsWow64 = %d\n",bIsWow64);
	ShowMsgBox("Before IAT Hook");
	IAT_InstallHook();
	ShowMsgBox("After  IAT Hook");
	IAT_UnInstallHook();
	ShowMsgBox("After  IAT Hook UnHooked");
	return 0;
}

//之所以把这个调用单独放在一个函数中，是因为Release模式下对调用进行了优化,第二次调用时直接采用了寄存器寻址而不是导入表
//因此，单独放在一个函数中可以避免这个情况。

VOID ShowMsgBox(char *szMsg)
{
	MessageBoxA(NULL,szMsg,"Test",MB_OK);
}


int WINAPI My_MessageBoxA(
	HWND hWnd,          // handle of owner window
	LPCTSTR lpText,     // address of text in message box
	LPCTSTR lpCaption,  // address of title of message box
	UINT uType          // style of message box
	)
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
	ret = OldMessageBox(hWnd,newText,newCaption,uType);//调用原MessageBox，并保存返回值
	//调用原函数之后，可以继续对OUT(输出类)参数进行干涉,比如网络函数的recv，可以干涉返回的内容
	return ret;//这里你还可以干涉原始函数的返回值
	
}

BOOL IAT_InstallHook()
{
	BOOL bResult = FALSE ;
	HMODULE hCurExe = GetModuleHandle(NULL);
	PULONG_PTR pt ;
	ULONG_PTR OrginalAddr;
	bResult = InstallModuleIATHook(hCurExe,"user32.dll","MessageBoxA",(PVOID)My_MessageBoxA,&pt,&OrginalAddr);
	if (bResult)
	{
		printf("[*]Hook安装完毕! pThunk=0x%p  OriginalAddr = 0x%p\n",pt,OrginalAddr);
		g_PointerToIATThunk = pt ;
		OldMessageBox = (PFN_MessageBoxA)OrginalAddr ;
	}
	return bResult;
	
}

VOID IAT_UnInstallHook()
{
	
	DWORD dwOLD;
	MEMORY_BASIC_INFORMATION  mbi;
	if (g_PointerToIATThunk)
	{
		//查询并修改内存页的属性
		VirtualQuery((LPCVOID)g_PointerToIATThunk,&mbi,sizeof(mbi));
		VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&dwOLD);
		//将原始的MessageBoxA地址填入IAT中
		*g_PointerToIATThunk = (ULONG)OldMessageBox;
		//恢复内存页的属性
		VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOLD,0);
	}

}

//************************************
// FullName:    InstallModuleIATHook
// Description: 为指定模块安装IAT Hook
// Access:      public 
// Returns:     BOOL
// Parameter:   HMODULE hModToHook , 待Hook的模块基址
// Parameter:   char * szModuleName , 目标函数所在模块的名字
// Parameter:   char * szFuncName , 目标函数的名字
// Parameter:   PVOID DetourFunc , Detour函数地址
// Parameter:   PULONG * pThunkPointer , 用以接收指向修改的位置的指针
// Parameter:   ULONG * pOriginalFuncAddr , 用以接收原始函数地址
//************************************
BOOL InstallModuleIATHook(
	HMODULE hModToHook,// IN
	char *szModuleName,// IN
	char *szFuncName,// IN
	PVOID DetourFunc,// IN
	PULONG_PTR *pThunkPointer,//OUT
	ULONG_PTR *pOriginalFuncAddr//OUT
	)
{
	PIMAGE_IMPORT_DESCRIPTOR  pImportDescriptor;
	PIMAGE_THUNK_DATA         pThunkData;
	ULONG ulSize;
	HMODULE hModule=0;
	ULONG_PTR TargetFunAddr;
	PULONG_PTR lpAddr;
	char *szModName;
	BOOL result = FALSE ;
	BOOL bRetn = FALSE;

	hModule = LoadLibrary(szModuleName);
	TargetFunAddr = (ULONG_PTR)GetProcAddress(hModule,szFuncName);
	printf("[*]Address of %s:0x%p\n",szFuncName,TargetFunAddr);
	printf("[*]Module To Hook at Base:0x%p\n",hModToHook);
	pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hModToHook, TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);;
	printf("[*]Find ImportTable,Address:0x%p\n",pImportDescriptor);
	while (pImportDescriptor->FirstThunk)
	{
		szModName = (char*)((PBYTE)hModToHook+pImportDescriptor->Name) ;
		printf("[*]Cur Module Name:%s\n",szModName);
		if (stricmp(szModName,szModuleName) != 0)
		{
			printf("[*]Module Name does not match, search next...\n");
			pImportDescriptor++;
			continue;
		}
		//程序的导入表处理完毕后OriginalFirstThunk可能是无效的，不能再根据名称来查找，而是遍历FirstThunk直接根据地址判断
		pThunkData = (PIMAGE_THUNK_DATA)((BYTE *)hModToHook + pImportDescriptor->FirstThunk);
		while(pThunkData->u1.Function)
		{
			lpAddr = (ULONG_PTR*)pThunkData;
			//找到了地址
			if((*lpAddr) == TargetFunAddr)
			{
				printf("[*]Find target address!\n");
				//通常情况下导入表所在内存页都是只读的，因此需要先修改内存页的属性为可写
				DWORD dwOldProtect;
				MEMORY_BASIC_INFORMATION  mbi;
				VirtualQuery(lpAddr,&mbi,sizeof(mbi));
				bRetn = VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&dwOldProtect);
				if (bRetn)
				{
					//内存页属性修改成功,继续下一步操作,先保存原始数据
					if (pThunkPointer != NULL)
					{
						*pThunkPointer = lpAddr ;
					}
					if (pOriginalFuncAddr != NULL)
					{
						*pOriginalFuncAddr = *lpAddr ;
					}
					//修改地址
					*lpAddr = (ULONG_PTR)DetourFunc;
					result = TRUE ;
					//恢复内存页的属性
					VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOldProtect,0);
					printf("[*]Hook ok.\n");
				}
				
				break;	
			}
			//---------
			pThunkData++;
		}
		pImportDescriptor++;
	}
	
	FreeLibrary(hModule);
	return result;
}
typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			// handle error
		}
	}
	return bIsWow64;
}

