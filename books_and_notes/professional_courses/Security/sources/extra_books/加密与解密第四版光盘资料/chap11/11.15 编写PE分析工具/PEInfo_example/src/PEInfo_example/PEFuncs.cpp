/*-----------------------------------------------------------------------
第11章  PE文件格式
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "stdafx.h"
#include "PEFuncs.h"

#include <imagehlp.h>


BOOL  LoadFileR(LPTSTR lpFilename,PMAP_FILE_STRUCT pstMapFile)
{

	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;

	memset(pstMapFile,0,sizeof(MAP_FILE_STRUCT));

	hFile=CreateFile(lpFilename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL,0);
	
	if (!hFile)				   
		return FALSE;

 hMapping=CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	if(!hMapping)
	{									
		CloseHandle(hFile);
		return FALSE;
	}
	ImageBase=MapViewOfFile(hMapping,FILE_MAP_READ,0,0,0);
    if(!ImageBase)
	{									
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return FALSE;
	}
	pstMapFile->hFile=hFile;
	pstMapFile->hMapping=hMapping;
	pstMapFile->ImageBase=ImageBase;
	return TRUE;
}

void UnLoadFile(PMAP_FILE_STRUCT pstMapFile)
{
	if(pstMapFile->ImageBase)
		UnmapViewOfFile(pstMapFile->ImageBase);
	
	if(pstMapFile->hMapping)
		CloseHandle(pstMapFile->hMapping);
	
	if(pstMapFile->hFile)
		CloseHandle(pstMapFile->hFile);
	
}
BOOL IsPEFile(LPVOID ImageBase)
{
    PIMAGE_DOS_HEADER  pDH=NULL;
    PIMAGE_NT_HEADERS  pNtH=NULL;
  
    if(!ImageBase)
	  return FALSE;
   
    pDH=(PIMAGE_DOS_HEADER)ImageBase;
    if(pDH->e_magic!=IMAGE_DOS_SIGNATURE)
         return FALSE;

    pNtH=(PIMAGE_NT_HEADERS32)((DWORD)pDH+pDH->e_lfanew);
    if (pNtH->Signature != IMAGE_NT_SIGNATURE )
        return FALSE;

    return TRUE;
	
}
//
PIMAGE_NT_HEADERS  GetNtHeaders(LPVOID ImageBase)
{
    
	if(!IsPEFile(ImageBase))
		return NULL;
	PIMAGE_NT_HEADERS  pNtH;
	PIMAGE_DOS_HEADER  pDH;
	pDH=(PIMAGE_DOS_HEADER)ImageBase;
	pNtH=(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);

	return pNtH;

}

//
PIMAGE_FILE_HEADER   GetFileHeader(LPVOID ImageBase)
{
    PIMAGE_DOS_HEADER  pDH=NULL;
    PIMAGE_NT_HEADERS  pNtH=NULL;
    PIMAGE_FILE_HEADER pFH=NULL;
    
    if(!IsPEFile(ImageBase))
		return NULL;
    pDH=(PIMAGE_DOS_HEADER)ImageBase;
    pNtH=(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);
    pFH=&pNtH->FileHeader;
    return pFH;
}
PIMAGE_OPTIONAL_HEADER GetOptionalHeader(LPVOID ImageBase)
{
    PIMAGE_DOS_HEADER  pDH=NULL;
    PIMAGE_NT_HEADERS  pNtH=NULL;
    PIMAGE_OPTIONAL_HEADER pOH=NULL;
   
     if(!IsPEFile(ImageBase))
		return NULL;
    pDH=(PIMAGE_DOS_HEADER)ImageBase;
    pNtH=(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);
    pOH=&pNtH->OptionalHeader;
    return pOH;
}
PIMAGE_SECTION_HEADER GetFirstSectionHeader(LPVOID ImageBase)
{
  	PIMAGE_NT_HEADERS     pNtH=NULL;
    PIMAGE_SECTION_HEADER pSH=NULL;
    
    pNtH=GetNtHeaders(ImageBase);
   	pSH=IMAGE_FIRST_SECTION(pNtH);
 	return  pSH;
}

LPVOID RvaToPtr(PIMAGE_NT_HEADERS pNtH,LPVOID ImageBase,DWORD dwRVA)
{	
 	return ImageRvaToVa(pNtH,ImageBase,dwRVA,NULL);
		
}

LPVOID GetDirectoryEntryToData(LPVOID ImageBase,USHORT DirectoryEntry)
{
	DWORD dwDataStartRVA;
	LPVOID pDirData=NULL;
	PIMAGE_NT_HEADERS     pNtH=NULL;
	PIMAGE_OPTIONAL_HEADER pOH=NULL;

	pNtH=GetNtHeaders(ImageBase);
	if(!pNtH)
		return NULL;
	pOH=GetOptionalHeader(ImageBase);
	if(!pOH)
		return NULL;
    dwDataStartRVA=pOH->DataDirectory[DirectoryEntry].VirtualAddress;
      if(!dwDataStartRVA)
        return NULL;
  
	pDirData=RvaToPtr(pNtH,ImageBase,dwDataStartRVA);
   if(!pDirData)
		return NULL;	 
   	return  pDirData;
}

PIMAGE_EXPORT_DIRECTORY  GetExportDirectory(LPVOID ImageBase)
{
   
	PIMAGE_EXPORT_DIRECTORY pExportDir=NULL;
	pExportDir=(PIMAGE_EXPORT_DIRECTORY)GetDirectoryEntryToData(ImageBase,IMAGE_DIRECTORY_ENTRY_EXPORT);
    if(!pExportDir)
		return NULL;	 
   	return  pExportDir;
}
PIMAGE_IMPORT_DESCRIPTOR  GetFirstImportDesc(LPVOID ImageBase)
{
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	pImportDesc=(PIMAGE_IMPORT_DESCRIPTOR)GetDirectoryEntryToData(ImageBase,IMAGE_DIRECTORY_ENTRY_IMPORT);
    if(!pImportDesc)
		return NULL;	 
   	return  pImportDesc;
}
DWORD   GetNumOfExportFuncs(LPVOID ImageBase,PIMAGE_EXPORT_DIRECTORY pExportDir)
{
	DWORD   dwnum=0;
    PDWORD pdwRvas=NULL;
	
/*	if(!IsPEFile(ImageBase))
		return NULL;
*/
 PIMAGE_NT_HEADERS pNtH=GetNtHeaders(ImageBase);
 
  pdwRvas=(PDWORD)RvaToPtr(pNtH,ImageBase,pExportDir->AddressOfFunctions);
	for(DWORD i=0;i<pExportDir->NumberOfFunctions;i++)
	{	
		if(*pdwRvas)
			++dwnum;
		
		++pdwRvas;
		
	} 
	return dwnum;
}

BOOL  IsDataDirPresent(LPVOID ImageBase,USHORT DirectoryEntry)
{

    if(!GetDirectoryEntryToData(ImageBase,DirectoryEntry))
		return FALSE;
	return TRUE;

}