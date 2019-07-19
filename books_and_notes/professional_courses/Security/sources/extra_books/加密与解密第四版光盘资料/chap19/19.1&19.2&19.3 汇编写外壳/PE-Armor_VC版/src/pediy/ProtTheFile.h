/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


#ifndef _PROTTHEFILE_H_
#define _PROTTHEFILE_H_
UINT ProtTheFileProc(HWND);

/*-------------------------------------------------------------*/
/*  ProtTheFile －     */
/*-------------------------------------------------------------*/
UINT ProtTheFile(HWND hDlg )
{
	ProtTheFileProc(hDlg);
	ISWORKING=FALSE;
	return TRUE;
}


UINT ProtTheFileProc(HWND hDlg )
{


	DWORD				    NumberOfBytesRW;
	PCHAR					MapOfSData = NULL;	

	PCHAR					pMapOfPackRes = NULL; //将特定的资源移到此处
	UINT					nNoPackResSize =NULL;

	PCHAR					m_pImportTable =NULL;//输入表加密临时的缓存
	UINT					m_pImportTableSize=NULL; //转换的输入表大小

	IMAGE_DOS_HEADER		dosHeader;
	IMAGE_NT_HEADERS		ntHeaders;
	UINT					nFileSize = 0;
	UINT					nSectionNum = 0;
	UINT					nImageSize = 0;
	UINT					nFileAlign = 0;
	UINT					nSectionAlign = 0;
	UINT					nHeaderSize = 0;
	UINT					nNtHeaderSize = 0;
	PIMAGE_SECTION_HEADER 	psecHeader = NULL;
	UINT					nRawDataOffset = 0;
	UINT					nRawDataSize = 0;
	UINT					nVirtualAddress = 0;
	UINT					nVirtualSize = 0;
	UINT					nIndex = 0;
	UINT                    nMapOfSDataSize =NULL; //额外数据大小
	DWORD					nbWritten;
	UINT					FirstResADDR =NULL;
	TCHAR					szFilebakName[MAX_PATH];						

	try
	{
		ISWORKING=TRUE;

		memset(szFilebakName,0,MAX_PATH);

		//创建备分文件

		if(IsCreateBak)
		{	
			strcpy(szFilebakName,szFilePath);
			strcat(szFilebakName,".bak");
			CopyFile(szFilePath,szFilebakName,FALSE);
		}


		//初始化
		m_pImageBase  = NULL;
		m_nImageSize  = 0;
		m_pntHeaders  = NULL;
		m_psecHeader  = NULL;


		//申请空间保存压缩过的区块信息，用于外壳在运行时解压缩

		m_pInfoData = NULL;
		m_pInfoSize = 0;
		m_pInfoData = new CHAR[SIZE_OF_PACKINFO];
		if (m_pInfoData == NULL)
		{
			AddLine(hDlg,"错误!内存不够!");
			return FALSE;
		}

		ZeroMemory(m_pInfoData, SIZE_OF_PACKINFO); 
		*(WORD *)(m_pInfoData) = 0;

		
  //把程序文件按照运行时映象的特点读入到内存中      	
		HANDLE hFile = CreateFile(
			szFilePath,
			GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE, 
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if ( hFile == INVALID_HANDLE_VALUE ) {
			 AddLine(hDlg,"错误!文件打开失败!");
			 return  FALSE;
		}
		// 读DOS头
		ReadFile(hFile,&dosHeader, sizeof(dosHeader), &NumberOfBytesRW, NULL);

		// 定位到PE头起始处e_lfanew
		SetFilePointer(hFile,dosHeader.e_lfanew,  NULL,  FILE_BEGIN);
		// 读出PE头
		ReadFile(hFile,&ntHeaders, sizeof(ntHeaders), &NumberOfBytesRW, NULL);

		// 获取文件大小等信息
		nFileSize     = GetFileSize(hFile,NULL);
		nSectionNum   = ntHeaders.FileHeader.NumberOfSections;
		nImageSize    = ntHeaders.OptionalHeader.SizeOfImage;
		nFileAlign    = ntHeaders.OptionalHeader.FileAlignment;
		nSectionAlign = ntHeaders.OptionalHeader.SectionAlignment;
		nHeaderSize   = ntHeaders.OptionalHeader.SizeOfHeaders;


		// 修正可能存在的映象大小没有对齐的情况
		m_nImageSize = AlignSize(nImageSize, nSectionAlign);
		
		// 申请内存用于保存映象
		m_pImageBase = new char[m_nImageSize];
			
		if (m_pImageBase == NULL)
		{
			AddLine(hDlg,"错误!内存不足!");
			return FALSE;
		}

		memset(m_pImageBase, 0, m_nImageSize);//将申请的内存区域初始化0
		//ZeroMemory(m_pImageBase, m_nImageSize);       


		// 首先定位并读PE文件头到内存中
		SetFilePointer(hFile, 0, NULL,  FILE_BEGIN);
		ReadFile(hFile, m_pImageBase, nHeaderSize,&NumberOfBytesRW, NULL);

		m_pntHeaders = (PIMAGE_NT_HEADERS)((DWORD)m_pImageBase + dosHeader.e_lfanew);

		// 注: 由于程序文件的IMAGE_DATA_DIRECTORY个数可以自定义(不一定非得定义16个)，
		// 因此这里通过计算来得到准确的IMAGE_NT_HEADERS的大小
		nNtHeaderSize = sizeof(ntHeaders.FileHeader) + sizeof(ntHeaders.Signature) + ntHeaders.FileHeader.SizeOfOptionalHeader;
		m_psecHeader = (PIMAGE_SECTION_HEADER)((DWORD)m_pntHeaders + nNtHeaderSize);

		// 先对SECTION数据进行修正
		for (nIndex = 0, psecHeader = m_psecHeader; nIndex < nSectionNum; ++ nIndex, ++ psecHeader)
		{
			nRawDataSize    = psecHeader->SizeOfRawData;
			nRawDataOffset  = psecHeader->PointerToRawData;
			nVirtualAddress = psecHeader->VirtualAddress;
			nVirtualSize    = psecHeader->Misc.VirtualSize;

			// 修正可能存在的对齐问题
			psecHeader->SizeOfRawData    = AlignSize(nRawDataSize, nFileAlign);
			psecHeader->Misc.VirtualSize = AlignSize(nVirtualSize, nSectionAlign);

			if (nIndex == nSectionNum - 1 && psecHeader->VirtualAddress + psecHeader->SizeOfRawData > m_nImageSize)
			{
				psecHeader->SizeOfRawData = m_nImageSize - psecHeader->VirtualAddress;
			}
		}

		// 循环依次读出SECTION数据到映象中的虚拟地址处
		for (nIndex = 0, psecHeader = m_psecHeader; nIndex < nSectionNum; ++ nIndex, ++ psecHeader)
		{
			nRawDataSize    = psecHeader->SizeOfRawData;
			nRawDataOffset  = psecHeader->PointerToRawData;
			nVirtualAddress = psecHeader->VirtualAddress;
			nVirtualSize    = psecHeader->Misc.VirtualSize;

			// 定位到SECTION数据起始处
			SetFilePointer(hFile, nRawDataOffset, NULL,  FILE_BEGIN);

			// 读SECTION数据到映象中
			ReadFile(hFile, &m_pImageBase[nVirtualAddress], nRawDataSize,&NumberOfBytesRW, NULL);
			
			if(IsDelShare)//忽略区块共享
			psecHeader->Characteristics = psecHeader->Characteristics & ~IMAGE_SCN_MEM_SHARED;

		}

		psecHeader -- ;

		// 修正可能存在的ImageSize没有对齐的情况
		m_pntHeaders->OptionalHeader.SizeOfImage = psecHeader->VirtualAddress + psecHeader->Misc.VirtualSize;

	//***********额外数据读取********************/
	/*保存地址：MapOfSData						*/
	/*大小：nMapOfSDataSize						*/
	/********************************************/
		if(IsSaveSData)
		{
			nMapOfSDataSize=nFileSize - (psecHeader->PointerToRawData + psecHeader->SizeOfRawData);
			if(nMapOfSDataSize>0)
			{
				MapOfSData = new char[nMapOfSDataSize];
				memset(MapOfSData , 0, nMapOfSDataSize);

				ReadFile(hFile, MapOfSData, nMapOfSDataSize,&NumberOfBytesRW, NULL);
				AddLine(hDlg,"额外数据读取完毕.");	
			}
			else
			{
				AddLine(hDlg,"没有额外数据.");
			}
		}
	
		CloseHandle(hFile);

		AddLine(hDlg,"文件读入完成.");	
	//***********去除重定位数据******************
		// 如果当前文件不是DLL文件，并且选择了"擦除重定位数据"时，擦除RELOC数据
		if (!(m_pntHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL)  && IsNoRelocation)
		{
			ClsRelocData();
		}
	//***********处理重定位数据（主要是针对DLL）******************
		if(SaveReloc())
			AddLine(hDlg,"重定位表处理成功.");

	//***********输入表加密******************

		if(IsProtImpTable)
		{
			//为简单，此处申请0xa0000内存存放新输入表（假设生成的新输入表结构尺寸小于0xa0000）
			m_pImportTable = new char[0xa0000]; 
			if (m_pImportTable == NULL)
			{
				AddLine(hDlg,"内存不足.");
				return FALSE;
			}
			ZeroMemory(m_pImportTable, 0xa0000); 
			m_pImportTableSize=MoveImpTable(m_pImportTable);

			if(m_pImportTableSize==FALSE){
				AddLine(hDlg,"处理输入表时指定的内存不足或程序没输入表.");
				return FALSE;
			}
		
			ClsImpTable();
			AddLine(hDlg,"输入表加密完成.");	

		}


	//***********移动不压缩资源******************

		if(isPackRes)
		{
			FirstResADDR = FindFirstResADDR();

			//计算特殊资源RT_ICON、RT_GROUP_ICON、RT_VERSION、XP风格的大小，申请缓存区，将其移过来
			//pMapOfPackRes = new char[0x10000];

			nNoPackResSize   = MoveRes(0x3,0,0) + MoveRes(0x0e,0,0) + MoveRes(0x10,0,0) + MoveRes(0x18,0,0);

			pMapOfPackRes = new char[nNoPackResSize];
			if (pMapOfPackRes == NULL)
			{
				AddLine(hDlg,"内存不足.");
				return FALSE;
			}
			ZeroMemory(pMapOfPackRes, nNoPackResSize); 

			UINT MoveResSize =NULL;
			MoveResSize = MoveRes(0x3 ,pMapOfPackRes,MoveResSize);//;RT_ICON	
			MoveResSize = MoveRes(0x0e,pMapOfPackRes,MoveResSize);//RT_GROUP_ICON
			MoveResSize = MoveRes(0x10,pMapOfPackRes,MoveResSize);//RT_VERSION
			MoveRes(0x18,pMapOfPackRes,MoveResSize);//XP风格
			AddLine(hDlg,"资源处理完成.");				
		}

	 //***********合并区段******************
		if(IsMergeSection)
			MergeSection();

	 //***********压缩区块数据******************
		AddLine(hDlg,"开始压缩数据……");
		if(!PackFile(szFilePath,FirstResADDR,hDlg))
		{
			AddLine(hDlg,"文件压缩失败!");
			return FALSE;
		}

	//*************处理外壳并写入******
		DisposeShell(pMapOfPackRes,nNoPackResSize,m_pImportTable,m_pImportTableSize,hDlg);

		
	//*************清空区块名吗******
		if(IsClsSecName)
			ClsSectionName();

	//*************重写文件头******
		SetFilePointer(hPackFile,0x0,  NULL,  FILE_BEGIN);
		nHeaderSize=m_pntHeaders->OptionalHeader.SizeOfHeaders ;// 读出PE头大小
		
		if(!WriteFile(hPackFile,(PCHAR)m_pImageBase,nHeaderSize,&nbWritten,NULL))
		{
			AddLine(hDlg,"错误!文件写失败!");
			CloseHandle(hPackFile);
			return FALSE;
		}

	//*************写入额外代码******

		if(IsSaveSData)
		{
			SetFilePointer(hPackFile, 0x0, NULL,  FILE_END);
			WriteFile(hPackFile,MapOfSData,nMapOfSDataSize,&nbWritten, NULL);
			AddLine(hDlg,"额外写入完成.");	

		}
	//*************加密完成,清理******	
		EnableWindow(GetDlgItem(hDlg,IDC_OPEN_BUTTON), TRUE);
		EnableWindow(GetDlgItem(hDlg,IDC_SET_PROPERTY), TRUE);
		
		CloseHandle(hPackFile);
		delete m_pInfoData;
		delete m_pImageBase;
		if(nMapOfSDataSize>0)
			delete MapOfSData;

		if(IsProtImpTable)
		{
			delete m_pImportTable;
			m_pImportTable = NULL;
		}

		if(isPackRes)
			pMapOfPackRes;
	}

	catch (...)
	{
	    AddLine(hDlg,"文件操作失败!");
		return FALSE;
	}

	AddLine(hDlg,"加密完成，谢谢使用！");	
	return TRUE;
}

#endif