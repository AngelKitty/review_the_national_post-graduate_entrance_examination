#ifndef _PEFUNCS_H_
#define _PEFUNCS_H_

typedef struct _MAP_FILE_STRUCT
{
	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;
}  MAP_FILE_STRUCT,* PMAP_FILE_STRUCT;
 
BOOL LoadFileR(LPTSTR lpFilename,PMAP_FILE_STRUCT pstMapFile);
void UnLoadFile(PMAP_FILE_STRUCT pstMapFile);
BOOL IsPEFile(LPVOID ImageBase);
PIMAGE_NT_HEADERS      GetNtHeaders(LPVOID ImageBase);
PIMAGE_FILE_HEADER     GetFileHeader(LPVOID ImageBase);
PIMAGE_OPTIONAL_HEADER GetOptionalHeader(LPVOID ImageBase);
PIMAGE_SECTION_HEADER  GetFirstSectionHeader(LPVOID ImageBase);
LPVOID RvaToPtr(PIMAGE_NT_HEADERS pNtH,LPVOID ImageBase,DWORD dwRVA);
LPVOID GetDirectoryEntryToData(LPVOID ImageBase,USHORT DirectoryEntry);
PIMAGE_EXPORT_DIRECTORY GetExportDirectory(LPVOID ImageBase);
PIMAGE_IMPORT_DESCRIPTOR  GetFirstImportDesc(LPVOID ImageBase);

DWORD   GetNumOfExportFuncs(LPVOID ImageBase,PIMAGE_EXPORT_DIRECTORY pExportDir);

BOOL    IsDataDirPresent(LPVOID ImageBase,USHORT DirectoryEntry);
#endif
