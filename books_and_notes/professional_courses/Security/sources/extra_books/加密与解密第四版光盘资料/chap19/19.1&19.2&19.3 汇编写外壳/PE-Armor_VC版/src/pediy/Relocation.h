/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


#ifndef _RELOCATION_H_
#define _RELOCATION_H_

/*-------------------------------------------------------------*/
/*  ClsRelocation                                              */
/* 清除重定位数据                                              */
/*-------------------------------------------------------------*/

void ClsRelocData( )
{
	
	PIMAGE_BASE_RELOCATION		pBaseReloc = NULL;
	PIMAGE_DATA_DIRECTORY		pRelocDir = NULL;
	UINT						nSize = 0;

	pRelocDir = &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	pBaseReloc =(PIMAGE_BASE_RELOCATION)RVAToPtr(pRelocDir->VirtualAddress);

	// 如果没有重定位数据，则直接返回
	if (pRelocDir->VirtualAddress == 0)
	{
		return ;
	}

	// 擦除所有的重定位数据
	while (pBaseReloc->VirtualAddress != 0)
	{
		nSize = pBaseReloc->SizeOfBlock;

		memset(pBaseReloc, 0 , nSize);

		pBaseReloc = (PIMAGE_BASE_RELOCATION)((DWORD)pBaseReloc + nSize);
	}
	m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = 0;
	m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = 0;

}
/*-------------------------------------------------------------*/
/*  SaveReloc                                                  */
/* 保存重定位数据，主要是支持DLL文件                           */
/*-------------------------------------------------------------*/

BOOL SaveReloc()
{


	//定义重定位表结构
	typedef struct _IMAGE_BASE_RELOCATION2 {
		DWORD   VirtualAddress;
		DWORD   SizeOfBlock;
		WORD    TypeOffset[1];
	} IMAGE_BASE_RELOCATION2;
	typedef IMAGE_BASE_RELOCATION2 UNALIGNED * PIMAGE_BASE_RELOCATION2;

	//新构造的重定位表结构
/*	typedef struct _NEWIMAGE_BASE_RELOCATION {
		BYTE   type;
		DWORD  FirstTypeRVA;
		BYTE   nNewItemOffset[1];
	} 
*/

	PIMAGE_DATA_DIRECTORY		pRelocDir = NULL;
	PIMAGE_BASE_RELOCATION2		pBaseReloc = NULL;

	PCHAR						pRelocBufferMap =NULL;
	PCHAR						pData = NULL;
	UINT						nRelocSize = NULL;
	UINT						nSize = 0;
	UINT						nType = 0;
	UINT						nIndex = 0;
	UINT						nTemp = 0;
	UINT						nNewItemOffset =0;
	UINT						nNewItemSize =0;


	pRelocDir = &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	nRelocSize = pRelocDir->Size;
	pBaseReloc =(PIMAGE_BASE_RELOCATION2)RVAToPtr(pRelocDir->VirtualAddress);

	// 如果没有重定位数据，则直接返回
	if (pRelocDir->VirtualAddress == 0)
	{
		return TRUE;
	}
	//申请临时缓存空间
	pRelocBufferMap = new char[nRelocSize];
	if (pRelocBufferMap == NULL)
		{
			return FALSE;
		}
	ZeroMemory(pRelocBufferMap, nRelocSize); 

	// 
	pData =pRelocBufferMap;
	
	while (pBaseReloc->VirtualAddress != 0)
	{
		nNewItemSize = (pBaseReloc->SizeOfBlock-8)/2;//保存新数据需要的字节长
		
		while (nNewItemSize != 0)
		{
			nType = pBaseReloc->TypeOffset[nIndex] >> 0x0c;//取type

			if(nType ==0x3 )
			{
				//取出ItemOffset，加上本段重定位起始地址 ，减去nTemp,得到的值准备放到新重定位表结构中
				nNewItemOffset = ((pBaseReloc->TypeOffset[nIndex] & 0x0fff) + pBaseReloc->VirtualAddress) - nTemp;
				if(nNewItemOffset > 0xff)//如果是本段重定位数据第一项
				{
					*(BYTE *)(pData) = 3;
					pData += sizeof(BYTE);
					*(DWORD *)pData = (DWORD)(nNewItemOffset);
					pData += sizeof(DWORD);
					
				}
				else
				{
					*(BYTE *)(pData) =(BYTE)(nNewItemOffset);
					pData += sizeof(BYTE);					
				}
				nTemp += nNewItemOffset;				
			}
			nNewItemSize--;	
			nIndex++;
		}

		nIndex     = 0;
		pBaseReloc = (PIMAGE_BASE_RELOCATION2)((DWORD)pBaseReloc + pBaseReloc->SizeOfBlock);
	}
	
	memset((PCHAR)RVAToPtr(pRelocDir->VirtualAddress),0,nRelocSize);
	memcpy((PCHAR)RVAToPtr(pRelocDir->VirtualAddress),pRelocBufferMap,nRelocSize);
	delete pRelocBufferMap;

	return TRUE;	
}

#endif