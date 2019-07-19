// PELoader.h: interface for the CLoadPE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PELOADER_H__02D71CD1_E8A1_41B5_B185_A841A7F59658__INCLUDED_)
#define AFX_PELOADER_H__02D71CD1_E8A1_41B5_B185_A841A7F59658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class PELoader  
{
public:
	PBYTE m_hModule;
	DWORD m_TotalImageSize;
	char m_szErrorMsg[1024];
	WORD m_SectionCnt;
	PIMAGE_DOS_HEADER m_pDosHeader;
	PIMAGE_FILE_HEADER m_pFileHeader;
	PIMAGE_DATA_DIRECTORY m_pRelocTable;
	PIMAGE_SECTION_HEADER m_pSecHeader;
	PIMAGE_EXPORT_DIRECTORY m_pExportDir;
	PIMAGE_IMPORT_DESCRIPTOR m_pImportDesp;
	PIMAGE_OPTIONAL_HEADER m_pOptHeader;
	DWORD m_dwEntryPoint;
	PBYTE m_MappedBase;
	DWORD Rav2Raw(DWORD VirtualAddr);
	DWORD GetTotalImageSize(DWORD Alignment);
	DWORD GetAlignedSize(DWORD theSize,DWORD Alignment);
	static DWORD _GetProcAddress(PBYTE pModule,char *szFuncName);
	PBYTE LoadPE(char *szPEPath,BOOL bDoReloc = TRUE,DWORD RelocBase = 0,BOOL bDoImport = FALSE);
	VOID FreePE(PBYTE pModule);
	VOID InitializePEHeaders(PBYTE pBase);
	VOID ProcessRelocTable(DWORD RelocBase);
	BOOL ProcessImportTable();
	PELoader();
	virtual ~PELoader();

private:
	BOOL SnapThunk(HMODULE hImpMode,PBYTE ImageBase, PIMAGE_THUNK_DATA NameThunk, PIMAGE_THUNK_DATA AddrThunk);
	char m_szPEPath[MAX_PATH];
	VOID Cleanup();
	PBYTE OpenFileAndMap(char *szPEFilePath);
	VOID FormatErrorMsg(char *szBuf,char *szPrompt, DWORD ErrCode);
	HANDLE m_hMap;
	HANDLE m_hFile;
};

#endif // !defined(AFX_LOADPE_H__02D71CD1_E8A1_41B5_B185_A841A7F59658__INCLUDED_)
