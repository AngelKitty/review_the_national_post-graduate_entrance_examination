/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


#ifndef _DISPOSESHELL_H_
#define _DISPOSESHELL_H_

/*-------------------------------------------------------------*/
/*  DisposeShell--开始处理外壳部分的数据   

各参数含义：
pMapOfPackRes:      图标等不能压缩的资源新地址
nNoPackResSize:     这些资源大小
m_pImportTable:     变形输入表的地址
m_pImportTableSize：变形输入表的大小
/*-------------------------------------------------------------*/

BOOL DisposeShell(PCHAR pMapOfPackRes,UINT nNoPackResSize,PCHAR m_pImportTable,UINT m_pImportTableSize ,HWND hDlg)
{

	PIMAGE_SECTION_HEADER	plastsecHeader = NULL;
	UINT					nBasePoint = 0;
	PIMAGE_DATA_DIRECTORY	pImportDir = NULL;
	UINT					nImportTableOffset=NULL;//Shell.asm中ImportTableBegin偏移值

	PIMAGE_SECTION_HEADER	psecHeader = m_psecHeader;
	PIMAGE_DATA_DIRECTORY	pTlsDir = NULL;
    PIMAGE_TLS_DIRECTORY	pTlsDirectory = NULL;


	PCHAR					mShell_pData= NULL;//外壳申请的地址（包括引导段和第二段）
	UINT					mShell_nSize = NULL;//外壳尺寸

	PDWORD					pShell0Data = NULL;
	DWORD					mShell0_nSize = NULL;//外壳引导段的尺寸

 	PCHAR					mShell1_pData= NULL;//外壳引导段空间
	UINT					mShell1_nSize = NULL;//外壳引导段尺寸

	PDWORD					pShellData = NULL;

	
	UINT					nRawSize = 0;
	UINT					nVirtualSize = 0;
	UINT					nFileAlign = 0;
	UINT					nSectionAlign = 0;
	DWORD                   nbWritten;

/************************************************************************/
/* 
/* -------------- <-ShellStart0
/* |  外壳引导  |
/* -------------- <-ShellEnd0
/* |  图标等资源| <------------pMapOfPackRes
/* -------------- <-ShellStart（以下数据压缩）
/* |  外壳第二段|
/* -------------- <-ShellEnd
/* |  变形输入表|
/* --------------
/************************************************************************/
	try
	{

        // 先处理外壳第二段
		mShell1_nSize =(DWORD) (&ShellEnd) - (DWORD)(&ShellStart);//计算外壳第二段尺寸

		mShell1_pData = new CHAR[mShell1_nSize + m_pImportTableSize];//申请大小＝外壳第二段＋变形输入表大小

		if (mShell1_pData == NULL)
		{
			AddLine(hDlg,"错误!内存不够!");
			return FALSE;
		}
		ZeroMemory(mShell1_pData, mShell1_nSize+ m_pImportTableSize); 
		
		memcpy(mShell1_pData, (&ShellStart), mShell1_nSize);//将外壳第二段读入缓冲
		
//由加壳程序修正的变量(外壳第二段)
/************************************************************************/
/* shell.asm中的变量

SHELL_DATA_1		LABEL	DWORD
0 OEP			    DD	0
1 S_IsProtImpTable	DD	0
2 ImpTableAddr		DD	0
3 S_FileIsDll		DD	0
4 S_RelocADDR		DD	0
5 S_PeImageBase     DD	0  ;原始映象基址
6 S_PackSection		DB	0a0h dup (?)

/************************************************************************/


		pShellData=(PDWORD)((DWORD)(&SHELL_DATA_1)-(DWORD)(&ShellStart)+mShell1_pData);//指向缓冲区中shell.asm的变量	
		pShellData[0] =m_pntHeaders->OptionalHeader.AddressOfEntryPoint;//将原始入口点保存到外壳中		
		pShellData[1] =IsProtImpTable;//将是否处理输入表的标志放到外壳里

		if(IsProtImpTable)
		{		
			memcpy(mShell1_pData+mShell1_nSize,m_pImportTable,m_pImportTableSize);
			pShellData[2]	= mShell1_nSize;//将变形输入表的地址（此处是相对外壳第二段偏移地址）保存 
			
		}
		else//将原始输入表地址保存到外壳中	
		{
			pImportDir		= &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
			pShellData[2]	= pImportDir->VirtualAddress;		
		}

		//将是否为DLL的标志放进外壳中
		if(m_pntHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL)
			pShellData[3] = 1;

		//将重定位地址放进外壳中
		pShellData[4] = m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress ;

		//保存原始映象基址到外壳中
		pShellData[5] = m_pntHeaders->OptionalHeader.ImageBase;

		//保存压缩块表信息到外壳中		
		memcpy((PCHAR)((PBYTE)(&pShellData[6])), m_pInfoData, m_pInfoSize);

		//对外壳第二段及变形输入表进行压缩
		//压缩后的地址 	  m_pPackData 
		//压缩后数据大小  m_nPackSize; 
		//mShell1_nSize+ m_pImportTableSize是外壳第二段＋变形输入表大小
		PackData(mShell1_pData, (mShell1_nSize+ m_pImportTableSize),hDlg);


	/******处理整个外壳段*******/



		mShell_nSize =(DWORD) (&ShellEnd0) - (DWORD)(&ShellStart0) + m_nPackSize + nNoPackResSize;//计算整个外壳需要尺寸

		mShell_pData = new CHAR[mShell_nSize];

		if (mShell_pData == NULL)
		{
			AddLine(hDlg,"错误!内存不够!");
			return FALSE;
		}
		ZeroMemory(mShell_pData, mShell_nSize); 

		mShell0_nSize    = (DWORD) (&ShellEnd0) - (DWORD)(&ShellStart0);
		memcpy(mShell_pData, &ShellStart0, mShell0_nSize);//将外壳引导段读入缓冲

		if(isPackRes)
		{
			memcpy((mShell_pData+mShell0_nSize),pMapOfPackRes,nNoPackResSize);//将不能压缩的资源（光标等）读入缓冲	
		}
		
		//将压缩后的数据读入缓冲(即将外壳的引导段与第二段合并)
		memcpy((mShell_pData+mShell0_nSize +nNoPackResSize), m_pPackData, m_nPackSize);


		/***修正外壳输入表***/
		PIMAGE_IMPORT_DESCRIPTOR	pImportDescriptor = NULL;
		PIMAGE_IMPORT_DESCRIPTOR	pDescriptor = NULL;
		UINT						nFirstThunk = 0;
		PIMAGE_THUNK_DATA32			pFirstThunk = NULL;
		UINT						nITRVA = 0;

	    psecHeader     = psecHeader + m_pntHeaders->FileHeader.NumberOfSections;//指向区块最尾端
		plastsecHeader = psecHeader - 1;
		nBasePoint     = plastsecHeader->VirtualAddress + plastsecHeader->Misc.VirtualSize;//新区块起点

		nImportTableOffset=(DWORD)(&ImportTableBegin)-(DWORD)(&ShellStart0);//ImportTableBegin在外壳引导段偏移值

		pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(mShell_pData+nImportTableOffset);//指向输入表起点
		nITRVA = nBasePoint +nImportTableOffset;//校正的参数

		for (pDescriptor = pImportDescriptor; pDescriptor->FirstThunk != 0; pDescriptor ++)
		{
			pDescriptor->OriginalFirstThunk += nITRVA;
			pDescriptor->Name += nITRVA;
			nFirstThunk = pDescriptor->FirstThunk;
			pDescriptor->FirstThunk = nFirstThunk + nITRVA;

			pFirstThunk = ( PIMAGE_THUNK_DATA32)(&(mShell_pData+nImportTableOffset)[nFirstThunk]);

			while (pFirstThunk->u1.AddressOfData != 0)
			{
				nFirstThunk = pFirstThunk->u1.Ordinal;
				pFirstThunk->u1.Ordinal = nFirstThunk + nITRVA;
				pFirstThunk ++;
			}
	
		}

//由加壳程序修正的变量(外壳引导段)
/************************************************************************/
/* shell.asm中的变量
SHELL_DATA_0		LABEL	DWORD
0 ShellBase			DD	0
1 ShellPackSize		DD	0
2 TlsTable			DB	18h dup (?)

  ImportTableEnd:
0 ShellBase			DD	0 //外壳部分相对 于ShellStart0偏移
1 ShellPackSize		DD	0 //外壳压缩部分的原始大小+变形输入表大小
2 Virtualalloc		DB	'VirtualAlloc',0
3 VirtualallocADDR	DD	0
4 TlsTable			DB	18h dup (?)
/************************************************************************/



		pShell0Data=(PDWORD)((DWORD)(&SHELL_DATA_0)-(DWORD)(&ShellStart0)+mShell_pData);//指向缓冲区中shell.asm的变量	
		pShell0Data[0] = mShell0_nSize + nNoPackResSize;
		pShell0Data[1] = mShell1_nSize + m_pImportTableSize;


		// 如果原来有TLS数据，则修正

		pTlsDir =&m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
		
		if (pTlsDir->VirtualAddress != NULL)
		{
			PDWORD		pShellTlsTable = NULL;
			pTlsDirectory = (PIMAGE_TLS_DIRECTORY32)RVAToPtr(pTlsDir->VirtualAddress);
			memcpy((PCHAR)(&pShell0Data[2]), pTlsDirectory, sizeof(IMAGE_TLS_DIRECTORY));
		
			m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress =nBasePoint + (DWORD)(&TlsTable)-(DWORD)(&ShellStart0);
			m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size = sizeof(IMAGE_TLS_DIRECTORY32);
						
		}



		nFileAlign    = m_pntHeaders->OptionalHeader.FileAlignment;
		nSectionAlign = m_pntHeaders->OptionalHeader.SectionAlignment;


		nRawSize     = AlignSize(mShell_nSize, nFileAlign);
		nVirtualSize = AlignSize(mShell_nSize, nSectionAlign);

		// 修正新区块信息
		memset(psecHeader, 0, sizeof(IMAGE_SECTION_HEADER));
		memcpy(psecHeader->Name, ".pediy", 6);
		psecHeader->PointerToRawData = plastsecHeader->PointerToRawData + plastsecHeader->SizeOfRawData;
		psecHeader->SizeOfRawData    = nRawSize;
		psecHeader->VirtualAddress   = plastsecHeader->VirtualAddress + plastsecHeader->Misc.VirtualSize;
		psecHeader->Misc.VirtualSize = nVirtualSize;
		psecHeader->Characteristics  = 0xE0000040;


		// 修改文件头
		m_pntHeaders->FileHeader.NumberOfSections ++;
		m_pntHeaders->OptionalHeader.CheckSum = 0;
		m_pntHeaders->OptionalHeader.SizeOfImage = psecHeader->VirtualAddress + psecHeader->Misc.VirtualSize;
		m_pntHeaders->OptionalHeader.AddressOfEntryPoint = nBasePoint;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = nBasePoint + nImportTableOffset;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = (DWORD)(&ImportTableEnd)-(DWORD)(&ImportTableBegin);
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress = 0;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size = 0;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress = 0;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size = 0;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress = 0;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size = 0;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress = 0;
		m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size = 0;

		//IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage)
		m_pntHeaders->OptionalHeader.DataDirectory[7].VirtualAddress = 0;
		m_pntHeaders->OptionalHeader.DataDirectory[7].Size = 0;

		//如是DLL，将外壳的重定位指向其虚构的重定位表处
		if(m_pntHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL)
		{
			m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = nBasePoint + (DWORD)(&RelocBaseBegin)-(DWORD)(&ShellStart0);
			m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = 0x08;
		}

		if(!WriteFile(hPackFile,(PCHAR)mShell_pData,mShell_nSize,&nbWritten,NULL))// 将外壳部分写入文件
		{
			AddLine(hDlg,"错误!文件写失败!");
			CloseHandle(hPackFile);
			return FALSE;
		}
		// 写入为对齐而填充的零数据
		if (nRawSize - mShell_nSize > 0)
		{
			FillZero(hPackFile, nRawSize - mShell_nSize);
		}

		delete[] mShell1_pData;		
		delete[] mShell_pData;


	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

#endif