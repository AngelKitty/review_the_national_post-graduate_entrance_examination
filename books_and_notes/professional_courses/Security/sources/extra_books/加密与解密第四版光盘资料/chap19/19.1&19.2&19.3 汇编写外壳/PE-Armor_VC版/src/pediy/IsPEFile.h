/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


#ifndef _ISPEFFILE_H_
#define _ISPEFFILE_H_

/*-------------------------------------------------------------*/
/* IsPEFile － 文件是有效的PE文件吗                            */
/*-------------------------------------------------------------*/

BOOL IsPEFile(TCHAR *szFilePath,HWND hDlg)
{

	DWORD					fileSize;
	HANDLE					hMapping;
	LPVOID					ImageBase;
    PIMAGE_DOS_HEADER	    pDosHeader=NULL;
    PIMAGE_NT_HEADERS       pNtHeader=NULL;
    PIMAGE_FILE_HEADER      pFilHeader=NULL;
	PIMAGE_OPTIONAL_HEADER  pOptHeader=NULL;
    PIMAGE_SECTION_HEADER   pSecHeader=NULL;


	//打开文件
  	HANDLE hFile = CreateFile(
		szFilePath,
		GENERIC_READ,
		FILE_SHARE_READ, 
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if ( hFile == INVALID_HANDLE_VALUE ) {
		 AddLine(hDlg,"错误!文件打开失败!");
		 return  FALSE;
	}
		
	//获得文件长度 :
	fileSize = GetFileSize(hFile,NULL);
	if (fileSize == 0xFFFFFFFF) {
		AddLine(hDlg,"错误!文件打开失败!");
		return FALSE;
	}

    hMapping=CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	if(!hMapping)
	{									
		CloseHandle(hFile);
		AddLine(hDlg,"错误!文件打开失败!");
		return FALSE;
	}
	ImageBase=MapViewOfFile(hMapping,FILE_MAP_READ,0,0,0);
    if(!ImageBase)
	{									
		CloseHandle(hMapping);
		CloseHandle(hFile);
	    AddLine(hDlg,"错误!文件打开失败!");
		return FALSE;
	}
   
    pDosHeader=(PIMAGE_DOS_HEADER)ImageBase;
    if(pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE){
		AddLine(hDlg,"错误!不是可执行文件!");
         return FALSE;
	}

    pNtHeader=(PIMAGE_NT_HEADERS32)((DWORD)pDosHeader+pDosHeader->e_lfanew);
    if (pNtHeader->Signature != IMAGE_NT_SIGNATURE ){
		AddLine(hDlg,"错误!不是可执行文件!");
        return FALSE;
	}

    pFilHeader=&pNtHeader->FileHeader;
    if (pFilHeader->NumberOfSections== 1 ){
		AddLine(hDlg,"文件可能已被压缩,放弃!");
        return FALSE;
	}

	 pOptHeader=&pNtHeader->OptionalHeader;//得到IMAGE_OPTIONAL_HEADER结构指针的函数
	// pOptHeader->AddressOfEntryPoint;

    //得到第一个区块的起始地址  
   	pSecHeader=IMAGE_FIRST_SECTION(pNtHeader);
    pSecHeader++;//得到第二个区块的起始地址
	if((pOptHeader->AddressOfEntryPoint) > (pSecHeader->VirtualAddress)){
		AddLine(hDlg,"文件可能已被压缩,放弃!");
        return FALSE;
	}

	 if (((pFilHeader->Characteristics) & IMAGE_FILE_DLL )!=0){
		 AddLine(hDlg,"是PE-DLL文件,可以压缩.");
	 }
	 else{
		 AddLine(hDlg,"是PE-EXE文件,可以压缩.");
	 }
	 UnmapViewOfFile(ImageBase);
	 CloseHandle(hMapping);
	 CloseHandle(hFile);
	
	 return TRUE;
}

#endif