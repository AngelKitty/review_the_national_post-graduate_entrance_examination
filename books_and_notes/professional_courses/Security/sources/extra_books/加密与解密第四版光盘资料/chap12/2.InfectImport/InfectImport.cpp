/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// InfectImport.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include "Image.h"

#define INFECT_SIG ('PE')

BOOL AddSectionTest();
BOOL InfectImport(
	IN char *szImageFilePath,
	IN char *szDllPath,
	IN char *szDllExportFunName
	);

int main(int argc, char* argv[])
{
#ifdef _WIN64
	InfectImport("F:\\Program2016\\DllInjection\\HostProc64.exe","Msgdll64.dll","Msg");
#else
	InfectImport("F:\\Program2016\\DllInjection\\HostProc.exe","Msgdll.dll","Msg");
#endif
	return 0;
}

BOOL InfectImport(
	IN char *szImageFilePath,
	IN char *szDllName,
	IN char *szDllExportFunName)
{
	CImage Img;
	BOOL bResult = FALSE ;
	WORD i = 0 ;
	DWORD dwIoCnt = 0 ;
	char szErrMsg[1024]={0};
	PIMAGE_SECTION_HEADER pImpSecHeader,pNewSecHeader = NULL,pTargetSecHeader = NULL;
	DWORD dwOldIIDCnt = 0 ,dwNewIIDCnt = 0 ;
	DWORD dwOldIIDSize = 0, dwNewIIDSize = 0 ;
	DWORD dwVAToStoreNewIID = 0 ; //新IID数组的存储位置
	DWORD dwnewThunkDataSize = 0 ; //新IID项的ThunkData的存储位置
	DWORD dwNewThunkDataVA = 0 ;//新IID项的ThunkData的存储位置
	DWORD dwSizeNeed = 0 ;
	DWORD dwThunkDataOffsetByIID = 0 ;
	BOOL bUseNewSection = FALSE ; //是否使用了新节
	BOOL bPlaceThunkDataToOldIID = TRUE ; //表明ThunkData存放的位置是不是在原来的IID位置，如果放不下，得找新位置
	
	printf("[*] Path = %s\n",szImageFilePath);
	//以读写方式打开目标文件
	HANDLE hFile = CreateFile(szImageFilePath,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//解析PE结构
	PBYTE pNotepad = Img.LoadImage(hFile,FALSE,0,FALSE);
	printf("[*] pImageBase = 0x%p\n",pNotepad);

	if (pNotepad == NULL)
	{
		printf("[-] 加载PE文件失败! %s\n",Img.GetErrorMsg(szErrMsg,1024));
		return FALSE;
	}
	
	//检查是否被感染过
	if (Img.m_pDosHeader->e_csum == INFECT_SIG)
	{
		printf("[-] 文件已经被感染过!\n");
		return FALSE;
	}
	printf("[*] 当前导入表信息 VA = 0x%p Size = 0x%X\n",Img.m_pImpDataDir->VirtualAddress,Img.m_pImpDataDir->Size);
	dwOldIIDSize = Img.m_pImpDataDir->Size ;
	dwOldIIDCnt = dwOldIIDSize / sizeof(IMAGE_IMPORT_DESCRIPTOR) ;
	dwNewIIDCnt = dwOldIIDCnt + 1;
	dwNewIIDSize = dwNewIIDCnt * sizeof(IMAGE_IMPORT_DESCRIPTOR) ;
	printf("[*] dwOldIIDCnt = %d  Size = 0x%X\n",dwOldIIDCnt,dwOldIIDSize);
	printf("[*] dwNewIIDCnt = %d  Size = 0x%X\n",dwNewIIDCnt,dwNewIIDSize);
	
	dwSizeNeed = dwNewIIDSize; //所需的大小是新导入表IID结构的大小

	pImpSecHeader = Img.LocateSectionByRVA(Img.m_pImpDataDir->VirtualAddress);
	printf("[*] 导入表所在节  %s  RawOffset = 0x%X Size = 0x%X\n",
		pImpSecHeader->Name,
		pImpSecHeader->PointerToRawData,
		pImpSecHeader->SizeOfRawData);
	
	DWORD dwPaddingSize = Img.GetSectionPhysialPaddingSize(pImpSecHeader);
	printf("[*] 导入表节空隙大小 = 0x%X\n",dwPaddingSize);
	
	//计算填充ThunkData需要的大小，它包括了OriginalFirstThunk、FirstThunk、IMPORT_BY_NAME，以及DllName
	dwnewThunkDataSize = sizeof(ULONG_PTR)*4 + strlen(szDllName) + 1 + sizeof(WORD) + strlen(szDllExportFunName) + 1 ;
	dwnewThunkDataSize = ALIGN_SIZE_UP(dwnewThunkDataSize,sizeof(ULONG)); //对齐
	//判断原导入表位置能否写下新的ThunkData
	if (dwnewThunkDataSize > dwOldIIDSize)
	{
		//写不下,那么在寻找节隙时就要加上
		//按ULONG_PTR对齐之后再添加ThunkData，虽然不按这个对齐也可以
		dwThunkDataOffsetByIID = ALIGN_SIZE_UP(dwNewIIDSize,sizeof(ULONG_PTR)) ;
		dwSizeNeed = dwThunkDataOffsetByIID + dwnewThunkDataSize ;
		bPlaceThunkDataToOldIID = FALSE ;
	}
	printf("[*] 放置新导入表数据所需要的大小 = 0x%X\n",dwSizeNeed);
	//dwPaddingSize = 0 ;//测试,强制添加新节
	if (dwPaddingSize >= dwSizeNeed)
	{
		printf("[*] 节空隙可以放下新的导入表，不需添加新节!\n");
		dwVAToStoreNewIID = pImpSecHeader->VirtualAddress + Img.GetAlignedSize(pImpSecHeader->Misc.VirtualSize,sizeof(DWORD));
		pTargetSecHeader = pImpSecHeader;
	}
	else
	{
		printf("[-] 节空隙不能放下新的导入表，需要添加新节!\n");
		//根据所需的空间大小添加一个新节
		pNewSecHeader = Img.AddNewSectionToFile(".Patch",dwSizeNeed);
		printf("[*] 新节添加完毕! VA = 0x%X  RawOffset = 0x%X  RawSize = 0x%X\n",
			pNewSecHeader->VirtualAddress,pNewSecHeader->PointerToRawData,pNewSecHeader->SizeOfRawData);
		dwVAToStoreNewIID = pNewSecHeader->VirtualAddress ;
		pTargetSecHeader = pNewSecHeader;
		bUseNewSection = TRUE;
	}
	
	//保存原导入表
	PIMAGE_IMPORT_DESCRIPTOR pOldImpDesp = Img.m_pImportDesp;
	PIMAGE_IMPORT_DESCRIPTOR pBuildNewImpDesp = (PIMAGE_IMPORT_DESCRIPTOR)malloc(dwSizeNeed);
	ZeroMemory(pBuildNewImpDesp,dwSizeNeed);
	//保存原来的导入表部分到新的中
	memcpy(pBuildNewImpDesp,pOldImpDesp,dwOldIIDSize);
	printf("[*] 原导入表IID结构保存完毕.\n");
	//指向一个新添加的IID项,稍后填充
	PIMAGE_IMPORT_DESCRIPTOR pNewImpEntry = pBuildNewImpDesp + dwOldIIDCnt - 1;


	//需要注意的是，ThunkData在32位和64位下的长度是不一样的，所以这里定义为自适应的ULONG_PTR
	PULONG_PTR pOriginalFirstThunk = NULL ;
	if (bPlaceThunkDataToOldIID)
	{
		//使用原IID的位置存放Thunk数据
		pOriginalFirstThunk = (PULONG_PTR)(Img.m_hModule + Img.m_pImpDataDir->VirtualAddress);
		dwNewThunkDataVA = Img.m_pImpDataDir->VirtualAddress ;
	}
	else
	{
		//原IID的位置存放不下，使用新位置存放
		pOriginalFirstThunk = (PULONG_PTR)((PBYTE)pBuildNewImpDesp + dwThunkDataOffsetByIID);
		dwNewThunkDataVA = dwVAToStoreNewIID + dwThunkDataOffsetByIID ; //在IID数据后面
	}
	ZeroMemory(pOriginalFirstThunk,dwnewThunkDataSize);
	//留出两项内容，第一项稍后填充，第二项填0作为结束标记
	PULONG_PTR pFirstThunk = pOriginalFirstThunk + 2 ;
	//留出两项内容,第一项稍后填充，第二项填0作为结束标记，之后作为Dll名称
	PCHAR  pDllName = (PCHAR)(pFirstThunk + 2);
	//保存dll名称
	strcpy(pDllName,szDllName);
	
	SIZE_T DllNameLen = strlen(szDllName);
	pDllName[DllNameLen] = 0;
	//接下来作为一个PIMPORT_BY_NAME结构
	PIMAGE_IMPORT_BY_NAME pImpName = (PIMAGE_IMPORT_BY_NAME)(pDllName + DllNameLen + 1);
	//填充它
	pImpName->Hint = 0;
	strcpy((char*)pImpName->Name,szDllExportFunName);
	printf("[*] 新导入表IID子结构构造完毕.\n");
	
	//计算结束位置
	PCHAR pEnd = (PCHAR)pImpName + sizeof(pImpName->Hint) + strlen((char*)pImpName->Name) + 1;
	//计算总占用的空间大小
	DWORD dwNewIIDEntrySizeUsed = (DWORD)pEnd - (DWORD)pOriginalFirstThunk;
	printf("[*] 新IID成员占用的空间大小 = 0x%X\n",dwNewIIDEntrySizeUsed);

	//返过来填充OriginalFirstThunk和FirstThunk
	//根据定义，OriginalFirst应指向IMAGE_IMPORT_BY_NAME结构的偏移
	pOriginalFirstThunk[0] = dwNewThunkDataVA + ((PBYTE)pImpName - (PBYTE)pOriginalFirstThunk);
	pFirstThunk[0] = pOriginalFirstThunk[0];

	//最后填充新的IID项，计算各项的RVA
	pNewImpEntry->OriginalFirstThunk = dwNewThunkDataVA;
	pNewImpEntry->Name = dwNewThunkDataVA + sizeof(ULONG_PTR)*4;//OriginalFirstThunk + FirstThunk的大小
	pNewImpEntry->FirstThunk = dwNewThunkDataVA + sizeof(ULONG_PTR)*2;
	printf("[*] 新IID填充完毕.\n");

	//更新PE头中的几个值
	//新的导入表大小
	Img.m_pImpDataDir->Size = dwNewIIDSize;
	//新的导入表IID的起始偏移
	Img.m_pImpDataDir->VirtualAddress = dwVAToStoreNewIID;
	if (!bUseNewSection)
	{
		pImpSecHeader->Misc.VirtualSize += dwSizeNeed;
	}
	
	//如果ThunkData放在了原IID的位置，需要设置节为可写的
	pImpSecHeader->Characteristics |= IMAGE_SCN_MEM_WRITE;
	//清空绑定输入表,强迫加载器重新加载IAT
	Img.m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
	Img.m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;
	//设置感染标记
	Img.m_pDosHeader->e_csum = INFECT_SIG;
	printf("[*] PE头更新完毕.\n");

	//写入文件
	printf("[*] 开始保存文件.\n");

	//开始保存内存中的修改内容到文件中
	//先写入新的PE头
	DWORD dwFileOffset = 0;
	ULONG_PTR dwVAInMemory = 0 ;
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);
	bResult = WriteFile(hFile,Img.m_hModule,Img.m_pOptHeader->SizeOfHeaders,&dwIoCnt,NULL);
	if (!bResult)
	{
		Img.FormatErrorMsg("[-] 写入文件失败!",GetLastError());
		return FALSE;
	}

	printf("[*] PE头写入完毕. Offset = 0x%X Size = 0x%x\n",dwFileOffset,dwIoCnt);

	//写入新IID的子结构信息,位置在原导入表的开始处
	dwVAInMemory = dwNewThunkDataVA ;
	dwFileOffset = Img.Rav2Raw(dwVAInMemory);
	SetFilePointer(hFile,dwFileOffset,NULL,FILE_BEGIN);
	bResult = WriteFile(hFile,pOriginalFirstThunk,dwNewIIDEntrySizeUsed,&dwIoCnt,NULL);
	if (!bResult)
	{
		Img.FormatErrorMsg("[-] 写入文件失败!",GetLastError());
		return FALSE;
	}
	printf("[*] 新IID项的子结构写入完毕. Offset = 0x%X Size = 0x%x\n",dwFileOffset,dwIoCnt);

	//写入新的IID结构
	dwVAInMemory = (ULONG_PTR)Img.m_pImpDataDir->VirtualAddress;
	dwFileOffset = Img.Rav2Raw(dwVAInMemory);
	SetFilePointer(hFile,dwFileOffset,NULL,FILE_BEGIN);
	bResult = WriteFile(hFile,pBuildNewImpDesp,dwNewIIDSize,&dwIoCnt,NULL);
	if (!bResult)
	{
		Img.FormatErrorMsg("[-] 写入文件失败!",GetLastError());
		return FALSE;
	}
	printf("[*] 新导入表整体写入完毕. Offset = 0x%X Size = 0x%x\n",dwFileOffset,dwIoCnt);
	printf("[*] 导入表感染完毕.\n");

	return TRUE;
}