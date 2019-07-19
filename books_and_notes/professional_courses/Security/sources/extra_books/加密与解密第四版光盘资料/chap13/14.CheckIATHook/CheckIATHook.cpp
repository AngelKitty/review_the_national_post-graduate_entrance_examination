/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// CheckIATHook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <imagehlp.h>
#include "PELoader.h"
#pragma comment(lib,"imagehlp.lib")



//以MessageBoxA的原型定义一个函数指针类型
typedef int 
(WINAPI *PFN_MessageBoxA)(
	HWND hWnd,          // handle of owner window
	LPCTSTR lpText,     // address of text in message box
	LPCTSTR lpCaption,  // address of title of message box
	UINT uType          // style of message box
	);


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
	PULONG *pThunkPointer,
	ULONG *pOriginalFuncAddr
	);

VOID ShowMsgBox(char *szMsg);
BOOL IAT_InstallHook();
VOID IAT_UnInstallHook();

//保存原始MessageBoxA的地址
PFN_MessageBoxA OldMessageBox=NULL;
//指向IAT中pThunk的地址
PULONG g_PointerToIATThunk = NULL;

VOID CheckModuleIATHook(HMODULE hMod);

int main(int argc, char *argv[ ])
{
	IAT_InstallHook();
	printf("[*] IAT Hook安装完毕. \n");
	ShowMsgBox("After  IAT Hook");
	
	CheckModuleIATHook(GetModuleHandle(NULL));
	return 0;
}

VOID CheckModuleIATHook(HMODULE hMod)
{
	PELoader Ldr;
	char szFilePath[MAX_PATH]={0};
	char *szImpModeName = NULL ;
	char *szFunName = NULL;

	printf("[*] 准备检测IAT Hook.\n");
	GetModuleFileName(hMod,szFilePath,MAX_PATH);
	PBYTE pNewMod = Ldr.LoadPE(szFilePath,FALSE,0,TRUE);//重定位不需要处理
	if (pNewMod == NULL)
	{
		printf("[*] 重加载PE失败!\n");
		return ;
	}

	printf("[*] 重加载后的Base = 0x%08X\n",pNewMod);
	
	PIMAGE_IMPORT_DESCRIPTOR  pOrgImportDescriptor = Ldr.m_pImportDesp;
	PIMAGE_IMPORT_DESCRIPTOR  pCurImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)Ldr.m_pImportDesp - (DWORD)Ldr.m_hModule + (DWORD)hMod );
	PIMAGE_THUNK_DATA pOrgThunk,pCurThunk,pNameThunk;
	PIMAGE_IMPORT_BY_NAME pImpName = NULL ;
	
	printf("[*] 开始检测IAT Hook.\n");
	//同步比较
	while (pOrgImportDescriptor->FirstThunk)
	{
		szImpModeName = (char*)pNewMod + pOrgImportDescriptor->Name;
		printf(" ->正在检测导入模块 : %s\n",szImpModeName);
		pOrgThunk = (PIMAGE_THUNK_DATA)((BYTE *)pNewMod + pOrgImportDescriptor->FirstThunk);
		pNameThunk = (PIMAGE_THUNK_DATA)((BYTE *)pNewMod + pOrgImportDescriptor->OriginalFirstThunk);
		pCurThunk = (PIMAGE_THUNK_DATA)((BYTE *)hMod + pOrgImportDescriptor->FirstThunk);
		while (pOrgThunk->u1.Function)
		{
			if (pOrgThunk->u1.Function != pCurThunk->u1.Function)
			{
				//这里没有判断按序号导入的情况，只处理了按名称导入
				pImpName = (PIMAGE_IMPORT_BY_NAME)((char*)pNewMod + pNameThunk->u1.AddressOfData) ;
				szFunName = (char*)pImpName->Name ;
				printf("   Found IAT Hook. 原始地址= 0x%08X  当前地址 = 0x%08X 函数名:%s\n",
					pOrgThunk->u1.Function,
					pCurThunk->u1.Function,
					szFunName);
			}
			pNameThunk++;
			pOrgThunk++;
			pCurThunk++;
		}
		pOrgImportDescriptor++;
	}

	printf("[*] IAT Hook检测完毕!\n");


	
}

VOID ShowMsgBox(char *szMsg)
{
	MessageBoxA(NULL,szMsg,"Test",MB_OK);
}


//之所以把这个调用单独放在一个函数中，是因为Release模式下对调用进行了优化,第二次调用时直接采用了寄存器寻址而不是导入表
//因此，单独放在一个函数中可以避免这个情况。

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
	PULONG pt ;
	ULONG OrginalAddr;
	bResult = InstallModuleIATHook(hCurExe,"user32.dll","MessageBoxA",(PVOID)My_MessageBoxA,&pt,&OrginalAddr);
	if (bResult)
	{
		printf("[*]Hook安装完毕! pThunk=0x%08X  OriginalAddr = 0x%08X\n",pt,OrginalAddr);
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
		VirtualQuery(g_PointerToIATThunk,&mbi,sizeof(mbi));
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
	PULONG *pThunkPointer,//OUT
	ULONG *pOriginalFuncAddr//OUT
	)
{
	PIMAGE_IMPORT_DESCRIPTOR  pImportDescriptor;
	PIMAGE_THUNK_DATA         pThunkData;
	ULONG ulSize;
	HMODULE hModule=0;
	ULONG TargetFunAddr;
	PULONG lpAddr;
	char *szModName;
	BOOL result = FALSE ;
	BOOL bRetn = FALSE;

	hModule = LoadLibrary(szModuleName);
	if (hModule == NULL)
	{
		return FALSE;
	}
	TargetFunAddr = (ULONG)GetProcAddress(hModule,szFuncName);
	if (TargetFunAddr == 0)
	{
		return FALSE;
	}
	//printf("[*]Address of %s:0x%08X\n",szFuncName,TargetFunAddr);
	//printf("[*]Module To Hook at Base:0x%08X\n",hModToHook);
	pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hModToHook, TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
	if (pImportDescriptor == NULL)
	{
		return FALSE;
	}
	//printf("[*]Find ImportTable,offset:0x%08X\n",pImportDescriptor);
	//PE程序加载完成后，OriginalFirstThunk可能是无效的，只能使用FirstThunk
	while (pImportDescriptor->FirstThunk)
	{
		szModName = (char*)((PBYTE)hModToHook+pImportDescriptor->Name) ;
		//printf("[*]Cur Module Name:%s\n",szModName);
		if (stricmp(szModName,szModuleName) != 0)
		{
			//printf("[*]Module Name does not match, search next...\n");
			pImportDescriptor++;
			continue;
		}
		//pThunkData指向内存中的IAT地址表
		pThunkData = (PIMAGE_THUNK_DATA)((BYTE *)hModToHook + pImportDescriptor->FirstThunk);
		while(pThunkData->u1.Function)
		{
			lpAddr = (ULONG*)pThunkData;
			//找到了地址
			if((*lpAddr) == TargetFunAddr)
			{
				//printf("[*]Find target address!\n");
				//通常情况下导入表所在内存页都是只读的，因此需要先修改内存页的属性为可写
				DWORD dwOldProtect;
				MEMORY_BASIC_INFORMATION  mbi;
				VirtualQuery(lpAddr,&mbi,sizeof(mbi));
				bRetn = VirtualProtect(mbi.BaseAddress,mbi.RegionSize,
					PAGE_EXECUTE_READWRITE,&dwOldProtect);
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
					*lpAddr = (ULONG)DetourFunc;
					result = TRUE ;
					//恢复内存页的属性
					VirtualProtect(mbi.BaseAddress,mbi.RegionSize,dwOldProtect,0);
					//printf("[*]Hook ok.\n");
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
