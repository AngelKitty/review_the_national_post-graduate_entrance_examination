/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#ifndef _PACKFILE_H_
#define _PACKFILE_H_

/*-------------------------------------------------------------*/
/*  pack_callback－ 进度条函数                                 */
/*-------------------------------------------------------------*/
int __cdecl pack_callback(UINT nSourcePos, UINT nDestinationPos,UINT nOutputbytes ,void *cbparam )
{
	UINT nRadio = 100-((nSourcePos - nDestinationPos)) * 100 /nSourcePos;

	SendMessage(hProgress, PBM_SETPOS, nRadio, 0);
		return 1;
	
}

/*-------------------------------------------------------------*/
/*  FillZero                                                   */
/* 填充零                                                      */
/*-------------------------------------------------------------*/

void FillZero(HANDLE hFile, UINT nLength)
{
	CHAR	ch = 0;
	DWORD   nbWritten;

	while (nLength -- > 0)
	{
		WriteFile(hFile,&ch,1,&nbWritten,NULL);
	}
}

/*-------------------------------------------------------------
 AddPackInfo                                                   
 记录压缩过的区块信息，用于外壳在运行时解压缩            
数据储存格式：
DWORD  保存区块原大小__解压所需空间大小
DWORD  保存区块原偏移__解压起点
DWORD  保存压缩后大小__解压数量

以后会保存在shell.asm变量：
S_PackSection	DB	0a0h dup (?)
-------------------------------------------------------------*/
BOOL AddPackInfo(UINT OriginalOffset,UINT OriginalSize,UINT nPackSize, HWND hDlg)
{
	try
	{
		if (m_pInfoData == NULL )
		{
			AddLine(hDlg,"无效的参数错误.");
			return FALSE;
		}

		if (m_pInfoSize + sizeof(UINT) * 2 > SIZE_OF_PACKINFO)
		{
			AddLine(hDlg,"缓冲区申请空间太小.");
			return FALSE;
		}


		*(UINT *)(&m_pInfoData[m_pInfoSize]) = OriginalSize ;//保存区块原大小__解压所需空间大小
		m_pInfoSize += sizeof(UINT);

		//保存区块原偏移__解压起点
		*(UINT *)(&m_pInfoData[m_pInfoSize]) = OriginalOffset ;
		m_pInfoSize += sizeof(UINT);
		
		// 记录压缩数据的大小
		*(UINT *)(&m_pInfoData[m_pInfoSize]) = nPackSize;
		m_pInfoSize += sizeof(UINT);
	}
	catch (...)
	{
		AddLine(hDlg,"未知异常.");
		return FALSE;
	}

	return TRUE;
}

/*-------------------------------------------------------------*/
/* PackData 												   */
/* 调用aplib压缩引擎压缩数据                                   */
/* 压缩后的地址:m_pPackData 大小：m_nPackSize (全局变量)       */
/*-------------------------------------------------------------*/

BOOL PackData(PCHAR pData, UINT nSize,HWND hDlg)
{
	PCHAR		pCloneData = NULL;
	UINT        m_nSpaceSize=NULL;

	try
	{
		if (pData == NULL || nSize <= 0 || IsBadReadPtr(pData, nSize))
		{
		    AddLine(hDlg,"无效的参数错误.");
			return FALSE;

		}

    	//	初始化

     	if (m_pWorkSpace != NULL)
		{
			delete[] m_pWorkSpace;	m_pWorkSpace = NULL;
		}

		if (m_pPackData != NULL)
		{
			delete[] m_pPackData;	m_pPackData = NULL;
		}

		m_nSpaceSize = 0;
		m_nPackSize = 0;




		// 计算工作空间大小
		m_nSpaceSize = aP_workmem_size(nSize);

		// 申请工作空间
		m_pWorkSpace = new CHAR[m_nSpaceSize];

		if (m_pWorkSpace == NULL)
		{
			AddLine(hDlg,"内存不足.");
			return FALSE;
		}

		// 申请保存压缩数据的空间
		m_pPackData = new CHAR[nSize * 2];

		if (m_pPackData == NULL)
		{
			AddLine(hDlg,"内存不足.");
			return FALSE;
		}

		// 申请空间
		pCloneData = (PCHAR)GlobalAlloc(GMEM_FIXED, nSize);

		if (pCloneData == NULL)
		{
			AddLine(hDlg,"内存不足.");
			return FALSE;
		}

		// 复制原始数据到新空间
		memcpy(pCloneData, pData, nSize);

		// 对原始数据进行压缩

		m_nPackSize = aP_pack((PBYTE)pCloneData, (PBYTE)m_pPackData, nSize, (PBYTE)m_pWorkSpace,pack_callback,0);

		// 释放新空间
		GlobalFree(pCloneData);
		pCloneData = NULL;

		if (m_nPackSize == 0)
		{
			AddLine(hDlg,"压缩过程中发现错误.");
			return FALSE;
		}
	}
	catch (...)
	{
		if (pCloneData != NULL)
		{
			GlobalFree(pCloneData);		pCloneData = NULL;
		}

			AddLine(hDlg,"未知异常.");
			return FALSE;

	}

			return TRUE;
}

/*-------------------------------------------------------------*/
/*  PackFile － 压缩PE    */
/*-------------------------------------------------------------*/
BOOL PackFile(TCHAR *szFilePath,UINT FirstResADDR,HWND hDlg)
{


	PIMAGE_SECTION_HEADER	psecHeader = m_psecHeader;

	UINT					nSectionNum = 0;
	UINT					nFileAlign = 0;
	UINT					nSectionAlign = 0;
	UINT					nSize = 0;
					
	DWORD                   nbWritten;
	UINT					nIndex = 0;
	PCHAR					pData = NULL;
	UINT					nNewSize = 0;
	UINT					nRawSize = 0;	
	
	try
	{		
		hPackFile = CreateFile(
			szFilePath,
			GENERIC_READ|GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE, 
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if ( hPackFile == INVALID_HANDLE_VALUE ) {
			AddLine(hDlg,"错误!文件打开失败!");
			return  FALSE;
			}

		if(IsFileAlignment200){
			m_pntHeaders->OptionalHeader.FileAlignment = 0x200;// 强制按最小200字节对齐
		}

		nSectionNum   = m_pntHeaders->FileHeader.NumberOfSections;
		nFileAlign    = m_pntHeaders->OptionalHeader.FileAlignment;
		nSectionAlign = m_pntHeaders->OptionalHeader.SectionAlignment;

		//将 绑定输入 清零
		PIMAGE_DATA_DIRECTORY			pBoundImportDir = NULL;
		PIMAGE_BOUND_IMPORT_DESCRIPTOR	pBoundImport = NULL;
		pBoundImportDir = &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT];
		if (pBoundImportDir->VirtualAddress != NULL && pBoundImportDir->Size > 0)
		{
			pBoundImport = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)RVAToPtr(pBoundImportDir->VirtualAddress);
			memset(pBoundImport,0, pBoundImportDir->Size);
			pBoundImportDir->VirtualAddress=0;
			pBoundImportDir->Size=0;	
		}
		
		//清除IAT信息
		PIMAGE_DATA_DIRECTORY			pIATDir = NULL;
		pIATDir=&m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT];

		if(pIATDir->VirtualAddress != NULL)
		{
			memset((PCHAR)RVAToPtr(pIATDir->VirtualAddress ),0,pIATDir->Size);
			pIATDir->VirtualAddress=0;
			pIATDir->Size=0;
		}
		
	
		nSize = (PCHAR)(&psecHeader[nSectionNum + 1]) - (PCHAR)m_pImageBase;// 计算新的文件头的大小(己考虑增加一个区段）

		nSize = AlignSize(nSize,nFileAlign);//对齐			
		m_pntHeaders->OptionalHeader.SizeOfHeaders = nSize;// 要修正文件头中的SizeOfHeaders大小
		
		psecHeader->PointerToRawData = nSize;// 同时还要修正第一个区块的RAW地址


		if(!WriteFile(hPackFile,(PCHAR)m_pImageBase,nSize,&nbWritten,NULL))// 写入文件
		{
			AddLine(hDlg,"错误!文件写失败!");
			CloseHandle(hPackFile);
			return FALSE;
		}

// 写入原各区块数据
	
		for (nIndex = 0; nIndex < nSectionNum; nIndex ++, psecHeader ++)
		{

			pData = RVAToPtr(psecHeader->VirtualAddress);
			//nSize = psecHeader->SizeOfRawData;如果不融合区块可以用这个
			nSize=psecHeader->Misc.VirtualSize;
		

			// 注: 由于某些区块由于之前的一些擦除操作已经清除了部分数据，导致区块可能变小，
			// 因此这里通过搜索并去掉尾部无用的零字节，重新计算区块的大小
			nNewSize = CalcMinSizeOfData(pData, nSize);

			// 如果整个区块已经只省零字节，则可以不需要保存此区块的数据
			if (nNewSize == 0)
			{
				psecHeader->SizeOfRawData = 0;
				psecHeader->Characteristics |= IMAGE_SCN_MEM_WRITE;

				// 由于压缩的原因，因此必须每次都修正下一个区块的起始偏移地址
				if (nIndex != nSectionNum - 1)
				{
					psecHeader[1].PointerToRawData = psecHeader->PointerToRawData + psecHeader->SizeOfRawData;
				}

				continue;
			}

			// 判断当前区块数据能否被压缩
			if (IsSectionCanPacked(psecHeader))
			{
				
				// 对区块数据进行压缩  
				/*压缩后的地址 	  m_pPackData  *****/
				/*压缩后数据大小  m_nPackSize; *****/
				if(PackData(pData, nNewSize,hDlg))
					AddLine(hDlg,"区块压缩成功");
				else
				{
					AddLine(hDlg,"压缩数据失败");
					return FALSE;
				}
			
				nRawSize  = AlignSize(m_nPackSize, nFileAlign);

				// 写入压缩后的数据
				if(!WriteFile(hPackFile,(PCHAR)m_pPackData,m_nPackSize,&nbWritten,NULL))// 写入文件
				{
					AddLine(hDlg,"错误!文件写失败!");
					CloseHandle(hPackFile);
					return FALSE;
				}


				// 写入为对齐而填充的零数据
				if (nRawSize - m_nPackSize > 0)
				{
					FillZero(hPackFile, nRawSize - m_nPackSize);
				}

				// 修正区块的大小
				psecHeader->SizeOfRawData = nRawSize;

				// 记录压缩过的区块信息，用于外壳在运行时解压缩

				if(!AddPackInfo(psecHeader->VirtualAddress,psecHeader->Misc.VirtualSize,psecHeader->SizeOfRawData, hDlg))
					return FALSE;
			}
			else
			{
				//对资源区段特殊处理
				if ((strcmp((char *)psecHeader->Name, ".rsrc") == 0)&& isPackRes)
				{
					PIMAGE_DATA_DIRECTORY			pResourceDir = NULL;
					UINT							nResourceDirSize = NULL;
					PCHAR							pResourcePtr = NULL;
					UINT							nResourceSize;
					pResourceDir = &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];

					if (pResourceDir->VirtualAddress != NULL)
					{
						pResourcePtr = (PCHAR)RVAToPtr(pResourceDir->VirtualAddress);
						nResourceSize = pResourceDir->Size;

						//写入资源段不被压缩的部分
						UINT					nFirstResSize;						
						PCHAR		pFirstResADDR = NULL;
						
						nResourceDirSize =  FirstResADDR  -pResourceDir->VirtualAddress ;//;减区块基址后得不压缩部分长度
						if(!WriteFile(hPackFile,(PCHAR)pResourcePtr,nResourceDirSize,&nbWritten,NULL))// 写入文件
						{
							AddLine(hDlg,"错误!文件写失败!");
							CloseHandle(hPackFile);
							return FALSE;
						}
						pFirstResADDR = RVAToPtr(FirstResADDR);  //待压缩资源地址
						nFirstResSize = nResourceSize-nResourceDirSize; //需要压缩资源的大小
						  
						/*压缩后的地址 	  m_pPackData  *****/
						/*压缩后数据大小  m_nPackSize; *****/
						if(PackData(pFirstResADDR,nFirstResSize,hDlg))
							AddLine(hDlg,"压缩资源成功.");
						else
							return FALSE;


	
						nRawSize  = AlignSize(m_nPackSize+nResourceDirSize, nFileAlign);//对齐后的资源段大小

						// 写入压缩后的数据
						if(!WriteFile(hPackFile,(PCHAR)m_pPackData,m_nPackSize,&nbWritten,NULL))// 写入文件
						{
							AddLine(hDlg,"错误!文件写失败!");
							CloseHandle(hPackFile);
							return FALSE;
						}


						// 写入为对齐而填充的零数据
						if (nRawSize - m_nPackSize -nResourceDirSize > 0)
						{
							FillZero(hPackFile, nRawSize - m_nPackSize -nResourceDirSize);
						}

						// 修正区块的大小
						psecHeader->SizeOfRawData = nRawSize;

						// 记录压缩过的区块信息，用于外壳在运行时解压缩

						if(!AddPackInfo(FirstResADDR,nFirstResSize,m_nPackSize, hDlg))
							return FALSE;					

						
					}
				}
				else
				{
				
					nRawSize  = AlignSize(nNewSize, nFileAlign);

					// 不能压缩的区块，则直接保存区块的数据
					if(!WriteFile(hPackFile,(PCHAR)pData,nRawSize,&nbWritten,NULL))// 写入文件
					{
						AddLine(hDlg,"错误!文件写失败!");
						CloseHandle(hPackFile);
						return FALSE;
					}
					psecHeader->SizeOfRawData = nRawSize;
				}
			}

			// 由于压缩的原因，因此必须每次都修正下一个区块的起始偏移地址
			if (nIndex != nSectionNum - 1)
			{
				psecHeader[1].PointerToRawData = psecHeader->PointerToRawData + psecHeader->SizeOfRawData;
			}

			psecHeader->Characteristics |= IMAGE_SCN_MEM_WRITE;

	  }	
	}
	catch (...)
	{
		return FALSE;
	}

		return TRUE;
}
#endif