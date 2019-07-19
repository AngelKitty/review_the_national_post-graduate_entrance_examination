/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#ifndef _RESOURCEDIR_H_
#define _RESOURCEDIR_H_


/*-------------------------------------------------------------*/
/*  FindFirstResADDR                                          */
/* 找各个资源保存地址最前的一个,以后部分可以压缩              */
/*-------------------------------------------------------------*/

UINT FindFirstResADDR()
{
	UINT							FirstResAddr = NULL;
	
	PIMAGE_DATA_DIRECTORY			pResourceDir = NULL;
	PIMAGE_RESOURCE_DIRECTORY		pResource = NULL;

	PIMAGE_RESOURCE_DIRECTORY		pTypeRes = NULL;
	PIMAGE_RESOURCE_DIRECTORY		pNameIdRes = NULL;
	PIMAGE_RESOURCE_DIRECTORY		pLanguageRes = NULL;

	PIMAGE_RESOURCE_DIRECTORY_ENTRY	pTypeEntry = NULL;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY	pNameIdEntry = NULL;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY	pLanguageEntry = NULL;

	PIMAGE_RESOURCE_DATA_ENTRY		pResData = NULL;

	UINT							nTypeNum = 0;
	UINT							nTypeIndex = 0;
	UINT							nNameIdNum = 0;
	UINT							nNameIdIndex = 0;
	UINT							nLanguageNum = 0;
	UINT							nLanguageIndex = 0;

	try
	{
		FirstResAddr = m_pntHeaders->OptionalHeader.SizeOfImage;//现放置一个最大值（这里取映像尺寸），然后根据比较逐渐减小

		pResourceDir = &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];
		if (pResourceDir->VirtualAddress == NULL)
		{
			return FALSE;
		}

		pResource = (PIMAGE_RESOURCE_DIRECTORY)RVAToPtr(pResourceDir->VirtualAddress);//资源起点地址
		pTypeRes    = pResource;
		nTypeNum    = pTypeRes->NumberOfIdEntries + pTypeRes->NumberOfNamedEntries;//该类型中有几类资源
		pTypeEntry  = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)pTypeRes + sizeof(IMAGE_RESOURCE_DIRECTORY));

		for (nTypeIndex = 0; nTypeIndex < nTypeNum; nTypeIndex ++, pTypeEntry ++)
		{
			
			//该类型目录地址
			pNameIdRes = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pResource + (DWORD)pTypeEntry->OffsetToDirectory);
			//该类型中有几个项目
			nNameIdNum = pNameIdRes->NumberOfIdEntries + pNameIdRes->NumberOfNamedEntries;
			pNameIdEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)pNameIdRes + sizeof(IMAGE_RESOURCE_DIRECTORY));


			for (nNameIdIndex = 0; nNameIdIndex < nNameIdNum; nNameIdIndex ++, pNameIdEntry ++)
			{
			
		
				//该项目目录地址
				pLanguageRes = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pResource + (DWORD)pNameIdEntry->OffsetToDirectory);
				nLanguageNum = pLanguageRes->NumberOfIdEntries + pLanguageRes->NumberOfNamedEntries;
				pLanguageEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)pLanguageRes + sizeof(IMAGE_RESOURCE_DIRECTORY));

				for (nLanguageIndex = 0; nLanguageIndex < nLanguageNum; nLanguageIndex ++, pLanguageEntry ++)
				{
					pResData = (PIMAGE_RESOURCE_DATA_ENTRY)((DWORD)pResource + (DWORD)pLanguageEntry->OffsetToData);

					if((pResData->OffsetToData < FirstResAddr) && (pResData->OffsetToData>pResourceDir->VirtualAddress))
					{
						FirstResAddr = pResData->OffsetToData;
					}
				}				
			}
		}

		
	}
	catch (...)
	{
		return FALSE;
	}
	return FirstResAddr;
	
}


/*-------------------------------------------------------------*/
/*  MoveRes													    */
/* 将特类型的资源移动到指定的位置	
ResType:资源的ID号；
MoveADDR：为目标地址,如为0，函数不移动数据，只返回数据大小   
MoveResSize:为上次移动资源的大小 */
/*-------------------------------------------------------------*/

BOOL MoveRes(UINT ResType,PCHAR MoveADDR,UINT MoveResSize)
{
	PIMAGE_DATA_DIRECTORY			pResourceDir = NULL;

	PIMAGE_RESOURCE_DIRECTORY		pResource = NULL;
	PIMAGE_RESOURCE_DIRECTORY		pTypeRes = NULL;
	PIMAGE_RESOURCE_DIRECTORY		pNameIdRes = NULL;
	PIMAGE_RESOURCE_DIRECTORY		pLanguageRes = NULL;

	PIMAGE_RESOURCE_DIRECTORY_ENTRY	pTypeEntry = NULL;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY	pNameIdEntry = NULL;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY	pLanguageEntry = NULL;

	PIMAGE_RESOURCE_DATA_ENTRY		pResData = NULL;
	UINT							nTypeNum = 0;
	UINT							nTypeIndex = 0;
	UINT							nNameIdNum = 0;
	UINT							nNameIdIndex = 0;
	UINT							nLanguageNum = 0;
	UINT							nLanguageIndex = 0;
	DWORD							mShell0_nSize = NULL;//外壳引导段的尺寸
	PCHAR							pOffsetToDataPtr;

	try
	{
		mShell0_nSize =(DWORD) (&ShellEnd0) - (DWORD)(&ShellStart0) ;//计算外壳引导段尺寸
		
		pResourceDir = &m_pntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE];
		if (pResourceDir->VirtualAddress == NULL)
		{
			return FALSE;
		}

		pResource = (PIMAGE_RESOURCE_DIRECTORY)RVAToPtr(pResourceDir->VirtualAddress);//资源起点地址
		pTypeRes    = pResource;
		nTypeNum    = pTypeRes->NumberOfIdEntries + pTypeRes->NumberOfNamedEntries;//该类型中有几类资源
		pTypeEntry  = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)pTypeRes + sizeof(IMAGE_RESOURCE_DIRECTORY));

		for (nTypeIndex = 0; nTypeIndex < nTypeNum; nTypeIndex ++, pTypeEntry ++)
		{
			if(pTypeEntry->NameIsString==0)
			{
			
				if((DWORD)pTypeEntry->NameOffset ==ResType)
				{
					//该类型目录地址
					pNameIdRes = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pResource + (DWORD)pTypeEntry->OffsetToDirectory);
					//该类型中有几个项目
					nNameIdNum = pNameIdRes->NumberOfIdEntries + pNameIdRes->NumberOfNamedEntries;
					pNameIdEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)pNameIdRes + sizeof(IMAGE_RESOURCE_DIRECTORY));

					for (nNameIdIndex = 0; nNameIdIndex < nNameIdNum; nNameIdIndex ++, pNameIdEntry ++)
					{			
					
						//该项目目录地址
						pLanguageRes = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pResource + (DWORD)pNameIdEntry->OffsetToDirectory);
						nLanguageNum = pLanguageRes->NumberOfIdEntries + pLanguageRes->NumberOfNamedEntries;
						pLanguageEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)pLanguageRes + sizeof(IMAGE_RESOURCE_DIRECTORY));

						for (nLanguageIndex = 0; nLanguageIndex < nLanguageNum; nLanguageIndex ++, pLanguageEntry ++)
						{
							//OffsetToData (RVA):
							pResData = (PIMAGE_RESOURCE_DATA_ENTRY)((DWORD)pResource + (DWORD)pLanguageEntry->OffsetToData);
						
							if(MoveADDR)
							{				
							
								pOffsetToDataPtr = RVAToPtr(pResData->OffsetToData);
							
								pResData->OffsetToData = m_nImageSize+mShell0_nSize+MoveResSize; //将OffsetToData字段指向外壳引导的新资源处

								memcpy(MoveADDR+MoveResSize,pOffsetToDataPtr,pResData->Size);
								ZeroMemory(pOffsetToDataPtr, pResData->Size); 				
							}
					
							MoveResSize+=pResData->Size;
						}											
					}
				return MoveResSize;
				}				
			}
		}	
	}
	catch (...)
	{
		return FALSE;
	}
	return 0;


}
#endif
