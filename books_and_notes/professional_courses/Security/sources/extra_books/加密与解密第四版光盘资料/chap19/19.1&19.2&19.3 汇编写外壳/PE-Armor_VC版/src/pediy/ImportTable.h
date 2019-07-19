/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


#ifndef _IMPORTTABLE_H_
#define _IMPORTTABLE_H_


/*------------------------------------------------------------- */
/*  输入表加密部分												*/
/* MoveImpTable                                                 */
/* 将一个程序的输入表的数据换种结构方式存储,以达到加密的目的.   */
/* 																*/
/*  -----------------------------------------------------		*/
/* | DWORD | byte | STRING | 00 |DWORD |BYTE |STRING|00|…		*/
/*  -----------------------------------------------------		*/
/* FirstThunk      DLL名      初始函数数目   函数名				*/
/*                          								    */
/*------------------------------------------------------------- */
// 函数返回值为加密生成数据的大小

UINT MoveImpTable(PCHAR m_pImportTable)
{

	PIMAGE_IMPORT_DESCRIPTOR	pImportDescriptor = NULL, pDescriptor = NULL;
	PIMAGE_DATA_DIRECTORY		pImportDir = NULL;
	PCHAR						pszDllName = NULL;
	UINT						nSize = 0;
	PCHAR						pData = NULL;
	PCHAR						pFunNum =NULL;
	PIMAGE_THUNK_DATA32			pFirstThunk = NULL;
	PIMAGE_IMPORT_BY_NAME		pImportName = NULL;

	try
	{
		pImportDir = &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
		if(0==pImportDir->VirtualAddress)
					return FALSE;			
		pImportDescriptor =(PIMAGE_IMPORT_DESCRIPTOR)RVAToPtr(pImportDir->VirtualAddress);

		// 遍历原始输入表
		for (pData = m_pImportTable, pDescriptor = pImportDescriptor; pDescriptor->Name != 0; pDescriptor ++)
		{
			// 保存首THUNK数据的RVA地址
			*(DWORD *)pData = pDescriptor->FirstThunk;
			pData += sizeof(DWORD);
			// 保存DLL名称长度(WORD)
			pszDllName = (PCHAR)RVAToPtr(pDescriptor->Name);
			*(BYTE *)(pData) = (BYTE)(strlen(pszDllName) ); //DLL字符串长度
			pData += sizeof(BYTE);
			//保存DLL字符串
			memcpy(pData, pszDllName, strlen(pszDllName) + 1);
			pData += strlen(pszDllName) + 1;
			pFunNum = pData; //pFunNum 指向需要初始函数的数目
			*(DWORD *)pFunNum =0;

			pData += sizeof(DWORD);//指向“BYTE |STRING|00|…”

			
			// 注: 通常程序在FirstThunk中保存函数名称等信息(标准形式)，但是部分程序可能为了加快程序启动，
			// 在FirstThunk中直接保存函数地址，在OriginalFirstThunk才保存函数名称等信息，因此这里只在OrginalFirstThunk
			// 无效时，才取FirstThunk中的数据作为函数名称等信息的存放位置。
			if (pDescriptor->OriginalFirstThunk != 0)
			{
				pFirstThunk = (PIMAGE_THUNK_DATA32)RVAToPtr(pDescriptor->OriginalFirstThunk);
			}
			else
			{
				
				pFirstThunk = (PIMAGE_THUNK_DATA32)RVAToPtr(pDescriptor->FirstThunk);
			}

			//……BYTE |STRING|00|……
			/*
			typedef struct _IMAGE_THUNK_DATA32 {
				union {
					PBYTE  ForwarderString;
					PDWORD Function;
				    DWORD  Ordinal;
					PIMAGE_IMPORT_BY_NAME  AddressOfData;
					} u1;
			} IMAGE_THUNK_DATA32;
			typedef IMAGE_THUNK_DATA32 * PIMAGE_THUNK_DATA32;*/

			while (pFirstThunk->u1.AddressOfData != NULL)
			{
				if (IMAGE_SNAP_BY_ORDINAL32(pFirstThunk->u1.Ordinal))//函数以序号方式
				{
					*(BYTE *)pData = 0;
					pData += sizeof(BYTE);

					*(DWORD *)pData = (DWORD)(pFirstThunk->u1.Ordinal & ~IMAGE_ORDINAL_FLAG32);//如果该元素值的最高二进位为1， 那么是序数
					pData += sizeof(DWORD)+1;
					(*(DWORD *)pFunNum) ++;//计数器，函数个数加1
				}
				else//函数以字符串类型
				{
					pImportName = (PIMAGE_IMPORT_BY_NAME)RVAToPtr((DWORD)(pFirstThunk->u1.AddressOfData));

						*(BYTE *)pData = (BYTE)(strlen((char *)pImportName->Name) );//函数名字符长度
						pData += sizeof(BYTE);

						memcpy(pData, pImportName->Name, strlen((char *)pImportName->Name) + 1);

						(*(DWORD *)pFunNum) ++;//计数器，函数个数加1

						pData += strlen((char *)pImportName->Name) + 1;
				
				}

				pFirstThunk ++;
				
			}
			
		}

		// DLL结构的结束符
		*(DWORD *)pData = (DWORD)0;
		pData += sizeof(DWORD);

		// 计算实际大小
		return(pData - m_pImportTable);

	}
	catch (...)
	{
		return FALSE;
	}

}
/*-------------------------------------------------------------*/
/*  ClsImpTable                                                */
/*  清除原输入表                                               */
/*-------------------------------------------------------------*/

void ClsImpTable( )
{

	PIMAGE_IMPORT_DESCRIPTOR	pImportDescriptor = NULL, pDescriptor = NULL;
	PIMAGE_DATA_DIRECTORY		pImportDir = NULL;
	PCHAR						pszDllName = NULL;
	PIMAGE_THUNK_DATA32			pFirstThunk = NULL;
	PIMAGE_IMPORT_BY_NAME		pImportName = NULL;

	pImportDir = &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	pImportDescriptor =(PIMAGE_IMPORT_DESCRIPTOR)RVAToPtr(pImportDir->VirtualAddress);

	// 遍历原始输入表
	for (pDescriptor = pImportDescriptor; pDescriptor->Name != 0; pDescriptor ++)//每循环一次，清除一个DLL的信息
	{
		pszDllName = (PCHAR)RVAToPtr(pDescriptor->Name);

		// 擦除DLL字符串信息
		memset(pszDllName, 0, strlen(pszDllName));

		// 擦除原始THUNK数据
		if (pDescriptor->OriginalFirstThunk != 0)
		{
			pFirstThunk = (PIMAGE_THUNK_DATA32)RVAToPtr(pDescriptor->OriginalFirstThunk);

			while (pFirstThunk->u1.AddressOfData != NULL)//清除OriginalFirstThunk
			{
				if (IMAGE_SNAP_BY_ORDINAL32(pFirstThunk->u1.Ordinal))
				{
					memset(pFirstThunk, 0 , sizeof(DWORD));
				}
				else
				{
					pImportName = (PIMAGE_IMPORT_BY_NAME)RVAToPtr((DWORD)(pFirstThunk->u1.AddressOfData));
					memset(pImportName, 0,strlen((char *)pImportName->Name) + sizeof(WORD));
					memset(pFirstThunk, 0 ,sizeof(DWORD));
				}

				pFirstThunk ++;
			}

		}
		
		pFirstThunk = (PIMAGE_THUNK_DATA32)RVAToPtr(pDescriptor->FirstThunk);
		//	清除FirstThunk
		while (pFirstThunk->u1.AddressOfData != NULL)
		{
			memset(pFirstThunk, 0 ,sizeof(DWORD));
			pFirstThunk ++;
		}

		memset(pDescriptor, 0 ,sizeof(IMAGE_IMPORT_DESCRIPTOR));
	}
}

#endif