// Stub.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Stub.h"


#pragma comment(linker, "/merge:.data=.text") 
#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/section:.text,RWE")
extern "C"__declspec(dllexport) GLOBAL_PARAM g_stcParam = { (DWORD)(Start) };

typedef void(*FUN)();
FUN g_oep;

ULONGLONG GetKernel32Addr()
{
	ULONGLONG dwKernel32Addr = 0;

	// 获取TEB的地址
	_TEB* pTeb = NtCurrentTeb();
	// 获取PEB的地址
	PULONGLONG pPeb = (PULONGLONG)*(PULONGLONG)((ULONGLONG)pTeb + 0x60); 
	// 获取PEB_LDR_DATA结构的地址
	PULONGLONG pLdr = (PULONGLONG)*(PULONGLONG)((ULONGLONG)pPeb + 0x18); 
	//模块初始化链表的头指针InInitializationOrderModuleList
	PULONGLONG pInLoadOrderModuleList = (PULONGLONG)((ULONGLONG)pLdr + 0x10);
	// 获取链表中第一个模块信息，exe模块
	PULONGLONG pModuleExe = (PULONGLONG)*pInLoadOrderModuleList; 
	// 获取链表中第二个模块信息，ntdll模块
	PULONGLONG pModuleNtdll = (PULONGLONG)*pModuleExe;    
	// 获取链表中第三个模块信息，Kernel32模块
	PULONGLONG pModuleKernel32 = (PULONGLONG)*pModuleNtdll;   
	// 获取kernel32基址
	dwKernel32Addr = pModuleKernel32[6];                     
	return dwKernel32Addr;
}

ULONGLONG MyGetProcAddress()
{
	ULONGLONG dwBase = GetKernel32Addr();
	// 1. 获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)dwBase;
	// 2. 获取NT头
	PIMAGE_NT_HEADERS64  pNt = (PIMAGE_NT_HEADERS64)(dwBase + pDos->e_lfanew);
	// 3. 获取数据目录表
	PIMAGE_DATA_DIRECTORY pExportDir = pNt->OptionalHeader.DataDirectory;
	pExportDir = &(pExportDir[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	DWORD dwOffset = pExportDir->VirtualAddress;
	// 4. 获取导出表信息结构
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)(dwBase + dwOffset);
	DWORD dwFunCount = pExport->NumberOfFunctions;
	DWORD dwFunNameCount = pExport->NumberOfNames;
	DWORD dwModOffset = pExport->Name;

	// Get Export Address Table
	PDWORD pEAT = (PDWORD)(dwBase + pExport->AddressOfFunctions);
	// Get Export Name Table
	PDWORD pENT = (PDWORD)(dwBase + pExport->AddressOfNames);
	// Get Export Index Table
	PWORD  pEIT = (PWORD)(dwBase + pExport->AddressOfNameOrdinals);

	for (DWORD dwOrdinal = 0; dwOrdinal < dwFunCount; dwOrdinal++)
	{
		if (!pEAT[dwOrdinal]) // Export Address offset
			continue;

		// 1. 获取序号
		DWORD dwID = pExport->Base + dwOrdinal;
		// 2. 获取导出函数地址
		ULONGLONG dwFunAddrOffset = pEAT[dwOrdinal];

		for (DWORD dwIndex = 0; dwIndex < dwFunNameCount; dwIndex++)
		{
			// 在序号表中查找函数的序号
			if (pEIT[dwIndex] == dwOrdinal)
			{
				// 根据序号索引到函数名称表中的名字
				ULONGLONG dwNameOffset = pENT[dwIndex];
				char* pFunName = (char*)((ULONGLONG)dwBase + dwNameOffset);
				if (!strcmp(pFunName, "GetProcAddress"))
				{// 根据函数名称返回函数地址
					return dwBase + dwFunAddrOffset;
				}
			}
		}
	}
	return 0;
}

void XorCode()
{
	// 获取代码基址
	PBYTE pBase = (PBYTE)((ULONGLONG)g_stcParam.dwImageBase + g_stcParam.lpStartVA);
    // 异或操作
	for (DWORD i = 0; i < g_stcParam.dwCodeSize; i++)
	{
		pBase[i] ^= g_stcParam.byXor;
	}
}

void  Start()
{
	// 获取kernel32基址
	fnGetProcAddress pfnGetProcAddress = (fnGetProcAddress)MyGetProcAddress();
	ULONGLONG dwBase = GetKernel32Addr();
	// 获取API地址
	fnLoadLibraryA pfnLoadLibraryA = (fnLoadLibraryA)pfnGetProcAddress((HMODULE)dwBase, "LoadLibraryA");
	fnGetModuleHandleA pfnGetModuleHandleA = (fnGetModuleHandleA)pfnGetProcAddress((HMODULE)dwBase, "GetModuleHandleA");
	fnVirtualProtect pfnVirtualProtect = (fnVirtualProtect)pfnGetProcAddress((HMODULE)dwBase, "VirtualProtect");
	HMODULE hUser32 = (HMODULE)pfnGetModuleHandleA("user32.dll");
	fnMessageBox pfnMessageBoxA = (fnMessageBox)pfnGetProcAddress(hUser32, "MessageBoxA");
	HMODULE hKernel32 = (HMODULE)pfnGetModuleHandleA("kernel32.dll");
	fnExitProcess pfnExitProcess = (fnExitProcess)pfnGetProcAddress(hKernel32, "ExitProcess");

    // 弹出信息框
	int nRet = pfnMessageBoxA(NULL, "欢迎使用免费64位加壳程序，是否运行主程序？", "Hello PEDIY", MB_YESNO);
	if (nRet == IDYES)
	{
	    // 修改代码段属性
	    ULONGLONG dwCodeBase = g_stcParam.dwImageBase + (DWORD)g_stcParam.lpStartVA;
	    DWORD dwOldProtect = 0;
	    pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	    XorCode(); // 解密代码
	    pfnVirtualProtect((LPBYTE)dwCodeBase, g_stcParam.dwCodeSize, dwOldProtect, &dwOldProtect);
		g_oep = (FUN)(g_stcParam.dwImageBase + g_stcParam.dwOEP);
		g_oep(); // 跳回原始OEP
	}
	// 退出程序
	pfnExitProcess(0);
}

