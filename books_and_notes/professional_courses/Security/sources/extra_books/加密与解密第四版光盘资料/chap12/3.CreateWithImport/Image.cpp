/*-----------------------------------------------------------------------
第12章  注入技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

// Image.cpp: implementation of the Image class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Image.h"
#include <shlwapi.h>

#pragma comment(lib,"shlwapi.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImage::CImage()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_hModule = NULL;
	m_pDosHeader = NULL;
	m_pFileHeader = NULL ;
	m_pRelocTable = NULL;
	m_pSecHeader = NULL;
	m_pExportDir = NULL;
	m_pImportDesp = NULL;
	m_pOptHeader = NULL;

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	m_dwPageSize = sysinfo.dwPageSize;
}

CImage::~CImage()
{
	Cleanup();
}

//************************************
// Method:    LoadPE
// FullName:  CImage::LoadPE
// Access:    public 
// Returns:   PBYTE
// Qualifier:
// Parameter: char * szPEPath , 待加载的PE模块的全路径
// Parameter: BOOL bDoReloc , 是否处理重定位
// Parameter: DWORD RelocBase , 重定位的基址，如果为0，则按实际加载位置重定位
// Parameter: BOOL bDoImport , 是否处理导入表
//************************************



PBYTE CImage::LoadImage(HANDLE hFile, BOOL bDoReloc, ULONG_PTR RelocBase, BOOL bDoImport)
{
	WORD i=0;
	BYTE *pMemory=NULL;
	BYTE *MappedBase = NULL;
	PIMAGE_SECTION_HEADER pTmpSecHeader = NULL ;
	BOOL bResult = FALSE ;
	DWORD dwFileSize = 0 ; //一般PE文件大小不会超过4G
	DWORD dwIoCnt = 0 ;

	__try
	{
		
		m_hFile = hFile;
		//获取文件大小
		dwFileSize = GetFileSize(m_hFile,NULL);
		if (dwFileSize == 0)
		{
			lstrcpy(m_szErrorMsg,"文件大小为0!");
			__leave;
		}
		
		//读取PE头
		DWORD dwSizeToRead = (dwFileSize > PEHEADER_SIZE) ? PEHEADER_SIZE:dwFileSize;
		ZeroMemory(m_HeaderData,PEHEADER_SIZE);
		bResult = ReadFile(m_hFile,m_HeaderData,dwSizeToRead,&dwIoCnt,NULL);
		if (!bResult)
		{
			FormatErrorMsg("读取文件失败!",GetLastError());
			__leave;
		}
		
		if (!VerifyImage(m_HeaderData))
		{
			lstrcpy(m_szErrorMsg,"不是有效的PE映像!");
			__leave;
		}

		//解析各个PE头部结构
		InitializePEHeaders(m_HeaderData);

		pTmpSecHeader = m_pSecHeader;
		//开始申请内存,为避免麻烦，这里直接申请可读可写可执行的内存
		pMemory = m_hModule = (BYTE*)VirtualAlloc(NULL,m_TotalImageSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		if (m_hModule == NULL)
		{
			lstrcpy(m_szErrorMsg,"内存不足，申请内存失败!");
			__leave;
		}
		//printf("m_hModule = 0x%X\n",m_hModule);
		//先拷贝PE头到申请的内存
		memcpy(pMemory,m_HeaderData,m_pOptHeader->SizeOfHeaders);
		pMemory += GetAlignedSize(m_pOptHeader->SizeOfHeaders,m_pOptHeader->SectionAlignment);
		
		//printf("Section  VirtualAddress VirtualSize   PointertoRawData  RawSize\n");
		//printf("=================================================================\n");
		LARGE_INTEGER liFileOffset;
		for (i=0;i< m_SectionCnt;i++)
		{
			liFileOffset.QuadPart = pTmpSecHeader->PointerToRawData;
			bResult = SetFilePointerEx(m_hFile,liFileOffset,NULL,FILE_BEGIN);
			if (!bResult)
			{
				FormatErrorMsg("设置文件读写位置失败!",GetLastError());
				__leave;
			}
			
			//读取各个节
			bResult = ReadFile(m_hFile,pMemory,pTmpSecHeader->SizeOfRawData,&dwIoCnt,NULL);
			if (!bResult)
			{
				FormatErrorMsg("读取文件失败!",GetLastError());
				__leave;
			}
			pMemory += GetAlignedSize(pTmpSecHeader->Misc.VirtualSize,m_pOptHeader->SectionAlignment);
			pTmpSecHeader++;
		}
		
		//重新解析PE头
		InitializePEHeaders(m_hModule);
		
		//开始处理重定位数据
		if (bDoReloc)
		{
			//如果RelocBase为0，则按实际加载位置进行重定位
			ULONG_PTR BaseToReloc = (RelocBase == 0 )?(DWORD)m_hModule : RelocBase ;
			ProcessRelocTable(BaseToReloc);
		}
		
		//处理导入表
		if (bDoImport)
		{
			ProcessImportTable();
		}

		bResult = TRUE; //加载成功
	}
	__finally
	{
		if (!bResult)
		{
			if (m_hFile != INVALID_HANDLE_VALUE)
			{
				CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		}
	}
	
	return m_hModule;
}

//以文件路径方式打开PE
PBYTE CImage::LoadImage(char *szPEPath, BOOL bDoReloc, ULONG_PTR RelocBase ,BOOL bDoImport)
{
	//保存PE路径
	lstrcpy(m_szPEPath,szPEPath);
	//以只读方式打开文件
	m_hFile = CreateFile(szPEPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		FormatErrorMsg("打开文件失败!",GetLastError());
		return NULL;
	}

	return LoadImage(m_hFile,bDoReloc,RelocBase,bDoImport);
}

VOID CImage::FreePE()
{
	VirtualFree(m_hModule,0,MEM_RELEASE);
	m_hModule = NULL;
}



DWORD CImage::GetAlignedSize(DWORD theSize, DWORD Alignment)
{
	DWORD dwAlignedVirtualSize=0;
	dwAlignedVirtualSize = ALIGN_SIZE_UP(theSize,Alignment);
	return dwAlignedVirtualSize;//返回对齐后的大小
}


ULONG_PTR CImage::GetAlignedPointer(ULONG_PTR uPointer, DWORD Alignment)
{
	DWORD dwAlignedAddress=0;
	dwAlignedAddress = ALIGN_SIZE_UP(uPointer,Alignment);
	return dwAlignedAddress;//返回对齐后的大小
}

/*
DWORD CImage::GetAlignedSize(DWORD theSize, DWORD Alignment)
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
*/

DWORD CImage::_GetProcAddress(PBYTE pModule, char *szFuncName)
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
	PIMAGE_NT_HEADERS pNtHeader;
	PIMAGE_OPTIONAL_HEADER pOptHeader;
	PIMAGE_EXPORT_DIRECTORY pExportDir;

	if (ModuleBase == NULL)
	{
		return 0;
	}

	pDosHeader=(PIMAGE_DOS_HEADER)ModuleBase;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return 0;
	}
	pNtHeader = (PIMAGE_NT_HEADERS)(ModuleBase+pDosHeader->e_lfanew);
	if (pNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		return 0 ;
	}

	pOptHeader = &pNtHeader->OptionalHeader;
	pExportDir=(PIMAGE_EXPORT_DIRECTORY)(ModuleBase+pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	namerav=(DWORD*)(ModuleBase+pExportDir->AddressOfNames);
	funrav=(DWORD*)(ModuleBase+pExportDir->AddressOfFunctions);
	nameOrdinal=(WORD*)(ModuleBase+pExportDir->AddressOfNameOrdinals);

	if ((DWORD)szFuncName < 0x0000FFFF)
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
			szMidName = ModuleBase+namerav[mid] ;
			cmpresult=strcmp(szFuncName,szMidName);
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

DWORD CImage::GetTotalImageSize(DWORD Alignment)
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
	return TotalSize;
}

DWORD CImage::Rav2Raw(DWORD VirtualAddr)
{
	DWORD RawAddr=0;
	if (VirtualAddr < m_pOptHeader->SizeOfHeaders)
	{
		RawAddr = VirtualAddr;
		return RawAddr;
	}
	PIMAGE_SECTION_HEADER pTmpSecHeader = LocateSectionByRVA(VirtualAddr);
	if (pTmpSecHeader != NULL)
	{
		RawAddr = VirtualAddr - pTmpSecHeader->VirtualAddress + pTmpSecHeader->PointerToRawData;
	}

	return RawAddr;
}

DWORD CImage::Raw2Rav(DWORD RawAddr)
{
	DWORD RavAddr=0;
	if (RawAddr < m_pOptHeader->SizeOfHeaders)
	{
		RavAddr = RawAddr;
		return RavAddr;
	}
	PIMAGE_SECTION_HEADER pTmpSecHeader = LocateSectionByRawOffset(RawAddr);
	if (pTmpSecHeader != NULL)
	{
		RavAddr = RawAddr - pTmpSecHeader->PointerToRawData + pTmpSecHeader->VirtualAddress;
	}
	
	return RavAddr;
}


VOID CImage::FormatErrorMsg(char *szPrompt, DWORD ErrCode)
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
	sprintf(m_szErrorMsg,"%s 错误代码:%d 原因:%s",szPrompt,ErrCode,(LPCTSTR)lpMsgBuf);
	LocalFree( lpMsgBuf );
}

VOID CImage::Cleanup()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE ;
	}
	
	if (m_hModule != NULL)
	{
		FreePE();
	}
}

VOID CImage::InitializePEHeaders(PBYTE pBase)
{
	//解析各个PE头部结构
	m_hModule = pBase ;
	m_pDosHeader =(PIMAGE_DOS_HEADER)pBase;
	m_pNtHeaders = (PIMAGE_NT_HEADERS)(pBase + m_pDosHeader->e_lfanew);
	m_pFileHeader = &m_pNtHeaders->FileHeader;
	m_SectionCnt = m_pFileHeader->NumberOfSections;
	m_pOptHeader = &m_pNtHeaders->OptionalHeader;
	m_pRelocTable = &(m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	m_pSecHeader = (PIMAGE_SECTION_HEADER)((BYTE*)m_pOptHeader+sizeof(IMAGE_OPTIONAL_HEADER));
	m_dwEntryPoint = m_pOptHeader->AddressOfEntryPoint;
	m_TotalImageSize = m_pOptHeader->SizeOfImage ;
	m_ImageBase = (ULONG_PTR)m_pOptHeader->ImageBase ;

	//导入表
	m_pImpDataDir = &m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	//因为导入表可能会被修改，所以先保存旧的导入表数据
	m_OldImpDir.VirtualAddress = m_pImpDataDir->VirtualAddress;
	m_OldImpDir.Size = m_pImpDataDir->Size;
	if (m_pImpDataDir->VirtualAddress != NULL)
	{
		m_pImportDesp = (PIMAGE_IMPORT_DESCRIPTOR)(pBase + m_pImpDataDir->VirtualAddress);
	}

	//导出表
	m_pExpDataDir = &m_pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
	if (m_pExpDataDir->VirtualAddress != NULL)
	{
		m_pExportDir = (PIMAGE_EXPORT_DIRECTORY)(pBase + m_pExpDataDir->VirtualAddress);
	}
	
	
	
}

VOID CImage::ProcessRelocTable(ULONG_PTR RelocBase)
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
			int numofReloc=(pRelocBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION))/2;
			//printf("Reloc Data num=%d\n",numofReloc);
			//重定位数据是16位的
			WORD offset=0;
			WORD *pRelocData=(WORD*)((BYTE*)pRelocBlock + sizeof(IMAGE_BASE_RELOCATION));
			for (i=0;i<numofReloc;i++)//循环，或直接判断*pData是否为0也可以作为结束标记
			{
				ULONG_PTR *RelocAddress=0;//需要重定位的地址
#ifdef _WIN64
				WORD RelocFlag = IMAGE_REL_BASED_DIR64 ;
#else
				WORD RelocFlag = IMAGE_REL_BASED_HIGHLOW ;
#endif
				//IMAGE_REL_BASED_DIR64
				//重定位的高4位是重定位类型，
				if (((*pRelocData)>>12) == RelocFlag)//判断重定位类型是否为IMAGE_REL_BASED_HIGHLOW,x86
				{
					//计算需要进行重定位的地址
					//重定位数据的低12位再加上本重定位块头的RAV即真正需要重定位的数据的RAV
					offset=(*pRelocData)&0xFFF;//小偏移
					RelocAddress=(ULONG_PTR*)(m_hModule+pRelocBlock->VirtualAddress+offset);
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

BOOL CImage::ProcessImportTable()
{
	BOOL bResult = TRUE ;
	char szPreDirectory[MAX_PATH]={0};
	char szCurDirectory[MAX_PATH]={0};
	char szPrompt[256]={0};
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
	
	while (pImportDescriptor->Name && pImportDescriptor->OriginalFirstThunk)
	{
		szImpModName = (char*)m_hModule + pImportDescriptor->Name ;
		hMod = LoadLibrary(szImpModName);
		if (hMod == NULL)
		{
			sprintf(szPrompt,"加载导入表模块 %s 失败!",szImpModName);
			FormatErrorMsg(szImpModName,GetLastError());
			return FALSE;
		}
		
		//printf("处理导入表模块 : %s\n",szImpModName);
		NameThunk = (PIMAGE_THUNK_DATA)(m_hModule + (ULONG)pImportDescriptor->OriginalFirstThunk);
		AddrThunk = (PIMAGE_THUNK_DATA)(m_hModule + (ULONG)pImportDescriptor->FirstThunk);
		
		while (NameThunk->u1.AddressOfData)
		{
			bResult = SnapThunk (hMod,szImpModName,m_hModule,NameThunk,AddrThunk);
			if (!bResult)
			{
				bResult = FALSE ;
				break;
			}
			NameThunk++;
			AddrThunk++;
		}
		
		if (!bResult)
		{
			break;
		}
		pImportDescriptor++;
    }
	
	SetCurrentDirectory(szPreDirectory);
	return bResult;
}

BOOL CImage::SnapThunk(HMODULE hImpMode,char *szImpModeName,PBYTE ImageBase, PIMAGE_THUNK_DATA NameThunk, PIMAGE_THUNK_DATA AddrThunk)
{
	BOOL bResult = FALSE ;
	PIMAGE_IMPORT_BY_NAME	  pImpName = NULL ;
	DWORD dwFunAddr = 0 ;
	ULONG Ordinal = 0 ;

	if (NameThunk->u1.AddressOfData & IMAGE_ORDINAL_FLAG32)
	{
		Ordinal = IMAGE_ORDINAL(NameThunk->u1.Ordinal);
		dwFunAddr = (DWORD)GetProcAddress(hImpMode,(LPCSTR)Ordinal);
		//printf("0x%08X 按序号导入 : %d\n",dwFunAddr,Ordinal);
		if (dwFunAddr == 0)
		{
			sprintf(m_szErrorMsg,"无法在导入模块%s中定位导入函数:%d (序号)",szImpModeName,Ordinal);
		}
	}
	else
	{
		pImpName = (PIMAGE_IMPORT_BY_NAME)(m_hModule + (ULONG)NameThunk->u1.AddressOfData);
		dwFunAddr = (DWORD)GetProcAddress(hImpMode,(LPCSTR)pImpName->Name);
		//printf("0x%08X 按名称导入 : %s\n",dwFunAddr,pImpName->Name);
		if (dwFunAddr == 0)
		{
			sprintf(m_szErrorMsg,"无法在导入模块%s中定位导入函数:%s ",szImpModeName,pImpName->Name);
		}
	}

	if (dwFunAddr != 0)
	{
		AddrThunk->u1.Function = dwFunAddr;
		bResult = TRUE;
	}

	return bResult;
}

//根据PE文件头，简单判断PE文件的有效性
BOOL CImage::VerifyImage(PVOID pBase)
{
	//解析各个PE头部结构
	m_pDosHeader =(PIMAGE_DOS_HEADER)pBase;
	if (m_pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	m_pNtHeaders = (PIMAGE_NT_HEADERS)((BYTE*)pBase + m_pDosHeader->e_lfanew);
	if (m_pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	
	return TRUE;
}

LPSTR CImage::GetErrorMsg(char *szBuf, int BufSize)
{
	int len = lstrlen(m_szErrorMsg);
	if (len <= BufSize )
	{
		lstrcpy(szBuf,m_szErrorMsg);
		return szBuf;
	}
	return NULL;
}

//根据相对虚拟地址查找所在的节
PIMAGE_SECTION_HEADER CImage::LocateSectionByRVA(DWORD dwRVA)
{
	WORD i = 0;
	PIMAGE_SECTION_HEADER pTemp = m_pSecHeader;
	for (i=0;i<m_SectionCnt;i++)
	{
		if (pTemp->VirtualAddress <= dwRVA
			&& dwRVA <(pTemp->VirtualAddress + pTemp->Misc.VirtualSize))
		{
			return pTemp;
		}
		pTemp++;
	}
	return NULL;
}

//根据文件偏移确定所在的节
PIMAGE_SECTION_HEADER CImage::LocateSectionByRawOffset(DWORD dwRawOffset)
{
	WORD i = 0;
	PIMAGE_SECTION_HEADER pTemp = m_pSecHeader;
	for (i=0;i<m_SectionCnt;i++)
	{
		if (pTemp->PointerToRawData <= dwRawOffset
			&& dwRawOffset <(pTemp->PointerToRawData + pTemp->SizeOfRawData))
		{
			return pTemp;
		}
		pTemp++;
	}
	return NULL;
}

//计算某个节按虚拟地址对齐后的空隙大小
DWORD CImage::GetSectionVirtualPaddingSize(PIMAGE_SECTION_HEADER pSecHeader)
{
	DWORD AlignedSize = GetAlignedSize(pSecHeader->Misc.VirtualSize,m_pOptHeader->SectionAlignment);
	return AlignedSize - pSecHeader->Misc.VirtualSize;
}

//计算某个节按虚拟地址对齐后的空隙大小
//VirtualSize和RawSize不确定哪个比较大
DWORD CImage::GetSectionPhysialPaddingSize(PIMAGE_SECTION_HEADER pSecHeader)
{
	DWORD dwPaddingSize = 0 ;
	if (pSecHeader->Misc.VirtualSize < pSecHeader->SizeOfRawData)
	{
		//节的内存大小小于文件大小
		/*
		.text name
		7748 virtual size
		1000 virtual address
		7800 size of raw data
		*/
		dwPaddingSize = pSecHeader->SizeOfRawData - pSecHeader->Misc.VirtualSize ;
	}
	else
	{
		//节的内存大小大于等于文件中的大小,则认为不存在空隙
		dwPaddingSize = 0 ;

	}
	return dwPaddingSize;
}

//默认情况下是以只读方式打开目标文件的,必要时转换为可写的
BOOL CImage::MakeFileHandleWritable()
{
	BOOL bResult = FALSE;
	HANDLE hNew = INVALID_HANDLE_VALUE;
	HANDLE hProc = OpenProcess(PROCESS_DUP_HANDLE,FALSE,GetCurrentProcessId());
	if (hProc == NULL)
	{
		return FALSE;
	}
	bResult = DuplicateHandle(
		hProc,
		m_hFile,
		hProc,
		&hNew,
		GENERIC_READ,
		FALSE,
		0
		);

	if (bResult)
	{
		CloseHandle(m_hFile);//关闭之前的Handle
		m_hFile = hNew;
	}
	else
	{
		FormatErrorMsg("更改句柄权限失败!",GetLastError());
	}
	
	CloseHandle(hProc);
	return bResult;
}

//解析当前内存中的PE结构
VOID CImage::AttachToMemory(PVOID pMemory)
{
	if (pMemory != NULL)
	{
		InitializePEHeaders((PBYTE)pMemory);
	}
}

//解析其它进程中的PE结构
BOOL CImage::AttachToProcess(HANDLE hProc ,PVOID ProcessImageBase)
{
	BOOL bResult = FALSE ;
	SIZE_T dwIoCnt = 0;
	m_hProc = hProc;
	m_ImageBase = (ULONG_PTR)ProcessImageBase;
	bResult = ReadProcessMemory(m_hProc,(LPVOID)m_ImageBase,m_HeaderData,0x1000,&dwIoCnt);
	if (!bResult)
	{
		FormatErrorMsg("ReadProcessMemory失败!",GetLastError());
		return FALSE;
	}

	//初始化PE结构
	InitializePEHeaders(m_HeaderData);
	return bResult;
}

//在文件中添加一个新节并返回新节的数据
PIMAGE_SECTION_HEADER CImage::AddNewSectionToFile(char *szSectionName, DWORD SectionSize)
{
	PIMAGE_SECTION_HEADER pNewSecHeader = m_pSecHeader + m_SectionCnt ;
	PIMAGE_SECTION_HEADER pLastSecHeader = m_pSecHeader + m_SectionCnt  - 1;
	DWORD dwSectionVA,dwSectionRawOffset,dwSectionSize;
	LARGE_INTEGER liFileOffset;
	BOOL bResult = FALSE ;
	DWORD dwIoCnt = 0 ;
	
	//计算新节的起始虚拟内存偏移
	dwSectionVA = pLastSecHeader->VirtualAddress + GetAlignedSize(pLastSecHeader->Misc.VirtualSize,m_pOptHeader->SectionAlignment);
	//计算新节的物理起始偏移
	dwSectionRawOffset = pLastSecHeader->PointerToRawData + GetAlignedSize(pLastSecHeader->SizeOfRawData,m_pOptHeader->FileAlignment);
	//计算新节的大小，按文件对齐粒度对齐
	dwSectionSize = GetAlignedSize(SectionSize,m_pOptHeader->FileAlignment);
	
	//设置文件指针位置
	liFileOffset.QuadPart = dwSectionRawOffset + dwSectionSize;
	bResult = SetFilePointerEx(m_hFile,liFileOffset,NULL,FILE_BEGIN);
	if (!bResult)
	{
		FormatErrorMsg("添加新节时设置文件指针错误!",GetLastError());
		return NULL;
		
	}

	bResult = SetEndOfFile(m_hFile);
	if (!bResult)
	{
		FormatErrorMsg("添加新节时设置文件结束位置错误!",GetLastError());
		return NULL;
		
	}
	
	//填充SectionHeader
	ZeroMemory(pNewSecHeader,sizeof(IMAGE_SECTION_HEADER));
	strncpy((char*)pNewSecHeader->Name,szSectionName,8);
	pNewSecHeader->Misc.VirtualSize = dwSectionSize;
	pNewSecHeader->VirtualAddress = dwSectionVA;
	pNewSecHeader->PointerToRawData = dwSectionRawOffset ;
	pNewSecHeader->SizeOfRawData = dwSectionSize;
	pNewSecHeader->Characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_EXECUTE;
	
	//更新PE头中的节个数
	m_pFileHeader->NumberOfSections += 1;
	m_SectionCnt++ ;
	//更新PE头中的总映像大小
	m_pOptHeader->SizeOfImage += dwSectionSize;

	//保存PE头到文件中
	liFileOffset.QuadPart = 0;
	bResult = SetFilePointerEx(m_hFile,liFileOffset,NULL,FILE_BEGIN);
	if (!bResult)
	{
		FormatErrorMsg("添加新节保存PE时设置文件指针错误!",GetLastError());
		return NULL;
		
	}

	bResult = WriteFile(m_hFile,m_hModule,m_pOptHeader->SizeOfHeaders,&dwIoCnt,NULL);
	if (!bResult)
	{
		FormatErrorMsg("添加新节保存PE时写入文件错误!",GetLastError());
		return NULL;
		
	}
	
	FlushFileBuffers(m_hFile);
	return pNewSecHeader;
}

//在内存中添加一个新节并返回新节的数据
PIMAGE_SECTION_HEADER CImage::AddNewSectionToMemory(char *szSectionName, DWORD SectionSize)
{
	PIMAGE_SECTION_HEADER pNewSecHeader = m_pSecHeader + m_SectionCnt ;
	PIMAGE_SECTION_HEADER pLastSecHeader = m_pSecHeader + m_SectionCnt  - 1;
	DWORD dwSectionVA,dwSectionRawOffset,dwSectionSize;
	BOOL bResult = FALSE ;
	SIZE_T dwIoCnt = 0 ;
	HANDLE hProc = (m_hProc == NULL )? GetCurrentProcess():m_hProc;
	ULONG_PTR  HighestUserAddress = 0;
	BYTE PEHeader[0x1000]={0};
	
	//获取基本的地址空间信息
	SYSTEM_INFO sysinfo;
	ZeroMemory(&sysinfo,sizeof(SYSTEM_INFO));
	GetSystemInfo(&sysinfo);
	HighestUserAddress = (ULONG_PTR)sysinfo.lpMaximumApplicationAddress;
	
	//计算新节的起始虚拟内存偏移
	dwSectionVA = pLastSecHeader->VirtualAddress + ALIGN_SIZE_UP(pLastSecHeader->Misc.VirtualSize,m_pOptHeader->SectionAlignment);
	//计算新节的物理起始偏移
	dwSectionRawOffset = pLastSecHeader->PointerToRawData + GetAlignedSize(pLastSecHeader->SizeOfRawData,m_pOptHeader->FileAlignment);
	//计算新节的大小，按文件对齐粒度对齐
	dwSectionSize = GetAlignedSize(SectionSize,m_pOptHeader->FileAlignment);
	
	ULONG_PTR dwNewSectionStartAddr = m_ImageBase + dwSectionVA;
	ULONG_PTR AddressToAlloc = GetAlignedPointer(dwNewSectionStartAddr,sysinfo.dwAllocationGranularity);
	PBYTE AllocatedMem = NULL ;
	//从PE最后一个节开始，向后申请内存
	for (AddressToAlloc = dwNewSectionStartAddr; AddressToAlloc < HighestUserAddress;AddressToAlloc += sysinfo.dwAllocationGranularity)
	{
		//申请地址
		AllocatedMem = (PBYTE)VirtualAllocEx(hProc,(PVOID)AddressToAlloc,dwSectionSize,MEM_RESERVE |MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		if (AllocatedMem != NULL)
		{
			break;
		}
	}
	
	if (AllocatedMem == NULL)
	{
		FormatErrorMsg("添加新节时在目标进程中申请内存失败!",GetLastError());
		return NULL;
	}
	//printf("[*] NewSection Address = 0x%p Size = 0x%X\n",AllocatedMem,dwSectionSize);
	dwSectionVA = MEM_OFFSET(AllocatedMem,m_ImageBase);

	//填充SectionHeader
	ZeroMemory(pNewSecHeader,sizeof(IMAGE_SECTION_HEADER));
	strncpy((char*)pNewSecHeader->Name,szSectionName,8);
	pNewSecHeader->Misc.VirtualSize = dwSectionSize;
	pNewSecHeader->VirtualAddress = dwSectionVA;
	pNewSecHeader->PointerToRawData = dwSectionRawOffset ;
	pNewSecHeader->SizeOfRawData = dwSectionSize;
	pNewSecHeader->Characteristics = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_EXECUTE;
	
	//更新PE头中的节个数
	m_pFileHeader->NumberOfSections += 1;
	m_SectionCnt++ ;
	//更新PE头中的总映像大小
	m_pOptHeader->SizeOfImage += dwSectionSize;
	
	
	//更新目标进程的PE头
	DWORD dwOldProtect = 0 ;
	bResult = VirtualProtectEx(hProc,(LPVOID)m_ImageBase,m_pOptHeader->SizeOfHeaders,PAGE_READWRITE,&dwOldProtect);
	if (!bResult)
	{
		FormatErrorMsg("修改目标进程内存属性时失败!",GetLastError());
		return NULL;
	}
	
	bResult = WriteProcessMemory(hProc,(LPVOID)m_ImageBase,m_HeaderData,m_pOptHeader->SizeOfHeaders,&dwIoCnt);
	if (!bResult)
	{
		FormatErrorMsg("向目标进程写入PE头数据时错误!",GetLastError());
		return NULL;
		
	}

	return pNewSecHeader;
}

PIMAGE_SECTION_HEADER CImage::ExtraLastSectionSizeToFile(DWORD SectionAddSize)
{
	PIMAGE_SECTION_HEADER pLastSecHeader = m_pSecHeader + m_SectionCnt  - 1;
	DWORD dwSectionNewVirtualSize,dwSectionNewRawOffset,dwSectionNewRawSize;
	DWORD dwOldSectionVirtualSize = 0 ;
	LARGE_INTEGER liFileOffset;
	BOOL bResult = FALSE ;
	DWORD dwIoCnt = 0 ;
	
	//在扩展最后一个节的情况下，需要更改最后一个节的RawSize和VirtualSize，起始偏移均不变
	//计算新节的物理大小，按文件对齐粒度对齐
	dwSectionNewRawOffset = pLastSecHeader->PointerToRawData ;
	dwSectionNewRawSize = GetAlignedSize(pLastSecHeader->SizeOfRawData + SectionAddSize,m_pOptHeader->FileAlignment);
	dwOldSectionVirtualSize = dwSectionNewVirtualSize =  GetAlignedSize(pLastSecHeader->Misc.VirtualSize , m_pOptHeader->SectionAlignment);
	//计处新节的VirtualSize大小,仅当内存大小小于文件大小时，需要增加
	if (pLastSecHeader->Misc.VirtualSize < dwSectionNewRawSize)
	{
		dwSectionNewVirtualSize += SectionAddSize;
	}
	
	//设置文件指针位置
	liFileOffset.QuadPart = dwSectionNewRawOffset +  pLastSecHeader->SizeOfRawData + SectionAddSize;
	bResult = SetFilePointerEx(m_hFile,liFileOffset,NULL,FILE_BEGIN);
	if (!bResult)
	{
		FormatErrorMsg("添加新节时设置文件指针错误!",GetLastError());
		return NULL;
		
	}
	
	bResult = SetEndOfFile(m_hFile);
	if (!bResult)
	{
		FormatErrorMsg("添加新节时设置文件结束位置错误!",GetLastError());
		return NULL;
		
	}
	
	//填充SectionHeader
	pLastSecHeader->Misc.VirtualSize = dwSectionNewVirtualSize;
	pLastSecHeader->SizeOfRawData = dwSectionNewRawSize;
	pLastSecHeader->Characteristics |=  IMAGE_SCN_MEM_READ;
	
	//更新PE头中的总映像大小
	m_pOptHeader->SizeOfImage = m_pOptHeader->SizeOfImage - dwOldSectionVirtualSize 
		+ GetAlignedSize(pLastSecHeader->Misc.VirtualSize,m_pOptHeader->SectionAlignment);
	
	//保存PE头到文件中
	liFileOffset.QuadPart = 0;
	bResult = SetFilePointerEx(m_hFile,liFileOffset,NULL,FILE_BEGIN);
	if (!bResult)
	{
		FormatErrorMsg("添加新节保存PE时设置文件指针错误!",GetLastError());
		return NULL;
		
	}
	
	bResult = WriteFile(m_hFile,m_hModule,m_pOptHeader->SizeOfHeaders,&dwIoCnt,NULL);
	if (!bResult)
	{
		FormatErrorMsg("添加新节保存PE时写入文件错误!",GetLastError());
		return NULL;
		
	}
	
	FlushFileBuffers(m_hFile);
	return pLastSecHeader;
	
}