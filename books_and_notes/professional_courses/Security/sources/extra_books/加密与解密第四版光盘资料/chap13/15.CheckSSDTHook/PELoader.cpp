/*-----------------------------------------------------------------------
第13章  Hook技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


// PELoader.cpp: implementation of the PELoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PELoader.h"
#include <shlwapi.h>

#pragma comment(lib,"shlwapi.lib")
#pragma comment(linker,"Base:0x40000000")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PELoader::PELoader()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_hMap = INVALID_HANDLE_VALUE;
	m_MappedBase = m_hModule = NULL;
	m_pDosHeader = NULL;
	m_pFileHeader = NULL ;
	m_pRelocTable = NULL;
	m_pSecHeader = NULL;
	m_pExportDir = NULL;
	m_pImportDesp = NULL;
	m_pOptHeader = NULL;
}

PELoader::~PELoader()
{
	Cleanup();
}

//************************************
// Method:    LoadPE
// FullName:  PELoader::LoadPE
// Access:    public 
// Returns:   PBYTE
// Qualifier:
// Parameter: char * szPEPath , 待加载的PE模块的全路径
// Parameter: BOOL bDoReloc , 是否处理重定位
// Parameter: DWORD RelocBase , 重定位的基址，如果为0，则按实际加载位置重定位
// Parameter: BOOL bDoImport , 是否处理导入表
//************************************
PBYTE PELoader::LoadPE(char *szPEPath, BOOL bDoReloc, DWORD RelocBase ,BOOL bDoImport)
{
	WORD i=0;
	BYTE *pMemory=NULL;
	BYTE *MappedBase = NULL;
	PIMAGE_SECTION_HEADER pTmpSecHeader = NULL ;
	
	//保存PE路径
	lstrcpy(m_szPEPath,szPEPath);

	//打开并映射
	MappedBase = OpenFileAndMap(szPEPath);
	if (MappedBase == NULL)
	{
		return NULL;
	}

	//解析各个PE头部结构
	InitializePEHeaders(MappedBase);
	pTmpSecHeader = m_pSecHeader;
	//开始申请内存
	m_TotalImageSize = GetTotalImageSize(m_pOptHeader->SectionAlignment);
	pMemory = m_hModule = (BYTE*)VirtualAlloc(NULL,m_TotalImageSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if (m_hModule == NULL)
	{
		return NULL;
	}
	//printf("m_hModule = 0x%X\n",m_hModule);
	//先拷贝PE头到申请的内存
	memcpy(pMemory,MappedBase,m_pOptHeader->SizeOfHeaders);
	pMemory += GetAlignedSize(m_pOptHeader->SizeOfHeaders,m_pOptHeader->SectionAlignment);

	//printf("Section  VirtualAddress VirtualSize   PointertoRawData  RawSize\n");
	//printf("=================================================================\n");
	char szTmpName[9]={0};
	for (i=0;i< m_SectionCnt;i++)
	{
		strncpy(szTmpName,(char*)pTmpSecHeader->Name,8);
		//printf("%8s %08X\t%08X\t%08X\t%08X\n",
		//szTmpName,pTmpSecHeader->VirtualAddress,pTmpSecHeader->Misc.VirtualSize,pTmpSecHeader->PointerToRawData,pTmpSecHeader->SizeOfRawData);
		//拷贝各个节
		//printf("[COPY] %8s\t:0x%08X Kernel=0x%08X Size=0x%08X\n",szTmpName,pMemory,KernelBase,GetAlignedSize(pTmpSecHeader->Misc.VirtualSize,pOptHeader->SectionAlignment));
		memcpy(pMemory,MappedBase+pTmpSecHeader->PointerToRawData,pTmpSecHeader->SizeOfRawData);
		pMemory+=GetAlignedSize(pTmpSecHeader->Misc.VirtualSize,m_pOptHeader->SectionAlignment);
		pTmpSecHeader++;
	}

	//重新解析PE头
	InitializePEHeaders(m_hModule);

	//开始处理重定位数据
	if (bDoReloc)
	{
		//如果RelocBase为0，则按实际加载位置进行重定位
		DWORD BaseToReloc = (RelocBase == 0 )?(DWORD)m_hModule : RelocBase ;
		ProcessRelocTable(BaseToReloc);
	}
	
	//处理导入表
	if (bDoImport)
	{
		ProcessImportTable();
	}

	//自行加载完毕，可以关闭映射了
	UnmapViewOfFile(MappedBase);
	CloseHandle(m_hMap);
	m_hMap = INVALID_HANDLE_VALUE ;
	return m_hModule;
}

VOID PELoader::FreePE(PBYTE pModule)
{
	VirtualFree(pModule,0,MEM_RELEASE);
	pModule = NULL;
}

DWORD PELoader::GetAlignedSize(DWORD theSize, DWORD Alignment)
{
	DWORD dwAlignedVirtualSize=0;
	DWORD moded=0,dived=0;
	dived=theSize/Alignment;
	moded=theSize%Alignment;
	if (moded)//有余数
	{
		dwAlignedVirtualSize=dived*Alignment;
		dwAlignedVirtualSize+=Alignment;
	}
	else
	{
		dwAlignedVirtualSize=theSize;
	}
	//printf("Recevid Size=%08X\tdived=%X\tmoded=%X\n",theSize,dived,moded);
	return dwAlignedVirtualSize;//返回对齐后的大小
}

DWORD PELoader::_GetProcAddress(PBYTE pModule, char *szFuncName)
{
	//自己实现GetProcAddress
	DWORD retAddr=0;
	DWORD *namerav,*funrav;
	DWORD cnt=0;
	DWORD max,min,mid;
	WORD *nameOrdinal;
	WORD nIndex=0;
	int cmpresult=0;
	char *ModuleBase=(char*)pModule;
	char *szMidName = NULL ;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_FILE_HEADER pFileHeader;
	PIMAGE_OPTIONAL_HEADER pOptHeader;
	PIMAGE_EXPORT_DIRECTORY pExportDir;

	if (ModuleBase == NULL)
	{
		return 0;
	}

	pDosHeader=(PIMAGE_DOS_HEADER)ModuleBase;
	pFileHeader=(PIMAGE_FILE_HEADER)(ModuleBase+pDosHeader->e_lfanew+4);
	pOptHeader=(PIMAGE_OPTIONAL_HEADER)((char*)pFileHeader+sizeof(IMAGE_FILE_HEADER));
	pExportDir=(PIMAGE_EXPORT_DIRECTORY)(ModuleBase+pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	namerav=(DWORD*)(ModuleBase+pExportDir->AddressOfNames);
	funrav=(DWORD*)(ModuleBase+pExportDir->AddressOfFunctions);
	nameOrdinal=(WORD*)(ModuleBase+pExportDir->AddressOfNameOrdinals);

	if ((DWORD)szFuncName<0x0000FFFF)
	{
		retAddr=(DWORD)(ModuleBase+funrav[(WORD)szFuncName]);
	}
	else
	{
		//二分法查找
		max = pExportDir->NumberOfNames ;
		min = 0;
		mid = (max+min)/2;
		while (min < max)
		{
			//printf("min = %d max = %d mid = %d ",min,max,mid);
			szMidName = ModuleBase+namerav[mid] ;
			cmpresult=strcmp(szFuncName,szMidName);
			//printf("Now[%d] : %s \n",mid,szMidName);
			if (cmpresult < 0)
			{
				//比中值小,则取中值-1为最大值
				max = mid -1 ;
			}
			else if (cmpresult > 0)
			{
				//比中值大,则取中值+1为最小值
				min = mid + 1;
			}
			else
			{
				break;
			}
			mid=(max+min)/2;
			
		}
		
		if (strcmp(szFuncName,ModuleBase+namerav[mid]) == 0)
		{
			nIndex=nameOrdinal[mid];
			retAddr=(DWORD)(ModuleBase+funrav[nIndex]);
		}
	}
	return retAddr;
}

PBYTE PELoader::OpenFileAndMap(char *szPEFilePath)
{
	//printf("正在打开文件和映射...\n");
	m_hFile=CreateFile(szPEFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(INVALID_HANDLE_VALUE == m_hFile) 
	{
		FormatErrorMsg(m_szErrorMsg,"打开文件失败!",GetLastError());
		goto __Failed;
	}
	m_hMap=CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if(m_hMap==NULL)
	{
		FormatErrorMsg(m_szErrorMsg,"创建文件映射失败!",GetLastError());
		goto __Failed;
	}
	m_MappedBase = (BYTE*)MapViewOfFile(m_hMap, FILE_MAP_READ, 0, 0, 0);
	if (m_MappedBase==NULL)
	{
		FormatErrorMsg(m_szErrorMsg,"映射文件失败!",GetLastError());
		goto __Failed;
	}
	CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE ;
	return m_MappedBase;
	
	
__Failed:
	Cleanup();
	return NULL;
}

DWORD PELoader::GetTotalImageSize(DWORD Alignment)
{
	DWORD TotalSize=0;
	DWORD tmp=0;
	PIMAGE_SECTION_HEADER pTmpSecHeader = m_pSecHeader;
	TotalSize+=GetAlignedSize(m_pOptHeader->SizeOfHeaders,Alignment);
	for (WORD i=0;i< m_SectionCnt;i++)
	{
		tmp=GetAlignedSize(pTmpSecHeader->Misc.VirtualSize,Alignment);
		TotalSize+=tmp;
		pTmpSecHeader++;
	}
	//printf("Total Size=0x%08X\n",TotalSize);
	return TotalSize;
}

DWORD PELoader::Rav2Raw(DWORD VirtualAddr)
{
	DWORD RawAddr=0;
	if (VirtualAddr < m_pOptHeader->SizeOfHeaders)
	{
		RawAddr=VirtualAddr;
		//printf("Rav2Raw 0x%08X\n",RawAddr);
		return RawAddr;
	}
	PIMAGE_SECTION_HEADER pTmpSecHeader = m_pSecHeader;
	for (WORD i=0;i < m_SectionCnt;i++)
	{
		//判断是否在某个区段内
		if (VirtualAddr>=(pTmpSecHeader->VirtualAddress)
			&&(VirtualAddr<(pTmpSecHeader->VirtualAddress+pTmpSecHeader->Misc.VirtualSize)))
		{
			RawAddr=pTmpSecHeader->PointerToRawData+VirtualAddr-pTmpSecHeader->VirtualAddress;
			return RawAddr;
		}
		pTmpSecHeader++;
	}
	return 0;
}

VOID PELoader::FormatErrorMsg(char *szBuf, char *szPrompt, DWORD ErrCode)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ErrCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	sprintf(szBuf,"%s ErrorCode:%d Reason:%s",szPrompt,ErrCode,(LPCTSTR)lpMsgBuf);
	LocalFree( lpMsgBuf );
}

VOID PELoader::Cleanup()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE ;
	}
	
	if (m_hMap != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hMap);
		m_hMap = INVALID_HANDLE_VALUE ;
	}

	if (m_hModule != NULL)
	{
		FreePE(m_hModule);
		m_hModule = NULL ;
	}
}

VOID PELoader::InitializePEHeaders(PBYTE pBase)
{
	//解析各个PE头部结构
	m_pDosHeader =(PIMAGE_DOS_HEADER)pBase;
	m_pFileHeader =(PIMAGE_FILE_HEADER)(pBase + m_pDosHeader->e_lfanew+4);
	m_SectionCnt = m_pFileHeader->NumberOfSections;
	m_pOptHeader = (PIMAGE_OPTIONAL_HEADER)((char*)m_pFileHeader+sizeof(IMAGE_FILE_HEADER));
	m_pRelocTable = &(m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	m_pSecHeader = (PIMAGE_SECTION_HEADER)((char*)m_pOptHeader+sizeof(IMAGE_OPTIONAL_HEADER));
	m_dwEntryPoint = (DWORD)pBase + m_pOptHeader->AddressOfEntryPoint;

	//导入表
	if (m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress != NULL)
	{
		m_pExportDir = (PIMAGE_EXPORT_DIRECTORY)(pBase + m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	}
	
	//导出表
	if (m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress != NULL)
	{
		m_pImportDesp = (PIMAGE_IMPORT_DESCRIPTOR)(pBase + m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	}
	
}

VOID PELoader::ProcessRelocTable(DWORD RelocBase)
{
	WORD i = 0 ;
	PIMAGE_BASE_RELOCATION pRelocBlock = NULL ;
	if (m_pRelocTable->VirtualAddress != NULL)
	{
		pRelocBlock=(PIMAGE_BASE_RELOCATION)(m_hModule + m_pRelocTable->VirtualAddress);
		//printf("After Loaded,Reloc Table=0x%08X\n",pRelocBlock);
		do
		{//处理一个接一个的重定位块，最后一个重定位块以RAV=0结束
			//需要重定位的个数，是本块的大小减去块头的大小，结果是以DWORD表示的大小
			//而重定位数据是16位的，那就得除以2
			int numofReloc=(pRelocBlock->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))/2;
			//printf("Reloc Data num=%d\n",numofReloc);
			//重定位数据是16位的
			WORD offset=0;
			WORD *pRelocData=(WORD*)((char*)pRelocBlock+sizeof(IMAGE_BASE_RELOCATION));
			for (i=0;i<numofReloc;i++)//循环，或直接判断*pData是否为0也可以作为结束标记
			{
				DWORD *RelocAddress=0;//需要重定位的地址
				//重定位的高4位是重定位类型，
				if (((*pRelocData)>>12)==IMAGE_REL_BASED_HIGHLOW)//判断重定位类型是否为IMAGE_REL_BASED_HIGHLOW
				{
					//计算需要进行重定位的地址
					//重定位数据的低12位再加上本重定位块头的RAV即真正需要重定位的数据的RAV
					offset=(*pRelocData)&0xFFF;//小偏移
					RelocAddress=(DWORD*)(m_hModule+pRelocBlock->VirtualAddress+offset);
					//对需要重定位的数据进行修正
					//修正方法:减去IMAGE_OPTINAL_HEADER中的基址，再加上新的基址即可
					*RelocAddress=*RelocAddress - m_pOptHeader->ImageBase + RelocBase;
				}
				pRelocData++;
				
			}
			//指向下一个重定位块
			pRelocBlock=(PIMAGE_BASE_RELOCATION)((char*)pRelocBlock+pRelocBlock->SizeOfBlock);
			
		}while (pRelocBlock->VirtualAddress);
	}
}

BOOL PELoader::ProcessImportTable()
{
	BOOL bResult = FALSE ;
	char szPreDirectory[MAX_PATH]={0};
	char szCurDirectory[MAX_PATH]={0};
	PIMAGE_IMPORT_DESCRIPTOR  pImportDescriptor = m_pImportDesp;
	PIMAGE_THUNK_DATA         NameThunk = NULL , AddrThunk = NULL;
	PIMAGE_IMPORT_BY_NAME	  pImpName = NULL ;
	HMODULE hMod = NULL ;
	char *szImpModName = NULL ;

	if (pImportDescriptor == NULL)
	{
		//无导入表，不需要处理
		return TRUE;
	}

	//更改当前路径，否则加载某些第三方dll时将找不到模块
	GetCurrentDirectory(MAX_PATH,szPreDirectory);
	lstrcpy(szCurDirectory,m_szPEPath);
	PathRemoveFileSpec(szCurDirectory);
	SetCurrentDirectory(szCurDirectory);
	
	//
	// Walk through the IAT and snap all the thunks.
	//
	
	while (pImportDescriptor->OriginalFirstThunk)
	{
		szImpModName = (char*)m_hModule + pImportDescriptor->Name ;
		hMod = LoadLibrary(szImpModName);
		if (hMod == NULL)
		{
			return FALSE;
		}
		
		//printf("处理导入表模块 : %s\n",szImpModName);
		NameThunk = (PIMAGE_THUNK_DATA)(m_hModule + (ULONG)pImportDescriptor->OriginalFirstThunk);
		AddrThunk = (PIMAGE_THUNK_DATA)(m_hModule + (ULONG)pImportDescriptor->FirstThunk);
		
		while (NameThunk->u1.AddressOfData)
		{
			bResult = SnapThunk (hMod,m_hModule,NameThunk,AddrThunk);
			NameThunk++;
			AddrThunk++;
		}

		pImportDescriptor++;
    }
	
	SetCurrentDirectory(szPreDirectory);
	return TRUE;
}

BOOL PELoader::SnapThunk(HMODULE hImpMode,PBYTE ImageBase, PIMAGE_THUNK_DATA NameThunk, PIMAGE_THUNK_DATA AddrThunk)
{
	BOOL bResult = FALSE ;
	PIMAGE_IMPORT_BY_NAME	  pImpName = NULL ;
	DWORD dwFunAddr = 0 ;
	ULONG Ordinal = 0 ;

	if (NameThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG32)
	{
		Ordinal = IMAGE_ORDINAL32(NameThunk->u1.Ordinal);
		dwFunAddr = (DWORD)GetProcAddress(hImpMode,(char*)Ordinal);
		//printf("0x%08X 按序号导入 : %d\n",dwFunAddr,Ordinal);
	}
	else
	{
		pImpName = (PIMAGE_IMPORT_BY_NAME)(m_hModule + (ULONG)NameThunk->u1.AddressOfData);
		dwFunAddr = (DWORD)GetProcAddress(hImpMode,(char*)pImpName->Name);
		//printf("0x%08X 按名称导入 : %s\n",dwFunAddr,pImpName->Name);
	}

	if (dwFunAddr != 0)
	{
		AddrThunk->u1.Function = dwFunAddr;
		bResult = TRUE;
	}

	return bResult;
}
