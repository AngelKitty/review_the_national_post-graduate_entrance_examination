// Image.h: interface for the CLoadPE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__02D71CD1_E8A1_41B5_B185_A841A7F59658__INCLUDED_)
#define AFX_IMAGE_H__02D71CD1_E8A1_41B5_B185_A841A7F59658__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

#define PEHEADER_SIZE (0x1000)
#define ALIGN_SIZE_UP(Size,Alignment)  (((ULONG_PTR)(Size) + Alignment - 1) & ~(Alignment - 1))
#define MEM_OFFSET(EndAddr,StartAddr) ((ULONG)((ULONG_PTR)EndAddr - (ULONG_PTR)StartAddr))

class CImage  
{
public:
	DWORD m_dwPageSize;
	HANDLE m_hFile;
	HANDLE m_hProc;
	WORD m_SectionCnt;
	//以下这些PE结构指针，只有当加载的PE是在当前进程时，才指向真实的内存
	//如果要操作的PE是在其它进程中，那么以下指针全部指向内部保存的PE头数据
	PBYTE m_hModule;
	PIMAGE_DOS_HEADER m_pDosHeader;
	PIMAGE_NT_HEADERS m_pNtHeaders;
	PIMAGE_FILE_HEADER m_pFileHeader;
	PIMAGE_OPTIONAL_HEADER m_pOptHeader;
	PIMAGE_DATA_DIRECTORY m_pRelocTable;
	PIMAGE_SECTION_HEADER m_pSecHeader;
	PIMAGE_DATA_DIRECTORY m_pImpDataDir;
	PIMAGE_DATA_DIRECTORY m_pExpDataDir;
	PIMAGE_EXPORT_DIRECTORY m_pExportDir;
	PIMAGE_IMPORT_DESCRIPTOR m_pImportDesp;
	IMAGE_DATA_DIRECTORY m_OldImpDir;
	//PE头中的非指针类数据
	ULONG_PTR m_dwEntryPoint;
	DWORD m_TotalImageSize;
	ULONG_PTR m_ImageBase;
	BYTE m_HeaderData[0x1000];//保存一份PE头的数据内部使用
	DWORD Rav2Raw(DWORD VirtualAddr);
	DWORD GetTotalImageSize(DWORD Alignment);
	DWORD GetAlignedSize(DWORD theSize,DWORD Alignment);
	ULONG_PTR GetAlignedPointer(ULONG_PTR uPointer,DWORD Alignment);
	static DWORD _GetProcAddress(PBYTE pModule,char *szFuncName);
	PBYTE LoadImage(HANDLE hFile, BOOL bDoReloc = TRUE,ULONG_PTR RelocBase = 0,BOOL bDoImport = FALSE);
	PBYTE LoadImage(char *szPEPath,BOOL bDoReloc = TRUE,ULONG_PTR RelocBase = 0,BOOL bDoImport = FALSE);
	VOID FreePE();
	VOID InitializePEHeaders(PBYTE pBase);
	VOID ProcessRelocTable(ULONG_PTR RelocBase);
	BOOL ProcessImportTable();
	VOID AttachToMemory(PVOID pMemory);
	BOOL AttachToProcess(HANDLE hProc,PVOID ProcessImageBase);
	BOOL MakeFileHandleWritable();
	DWORD Raw2Rav(DWORD RawAddr);
	DWORD GetSectionPhysialPaddingSize(PIMAGE_SECTION_HEADER pSecHeader);
	DWORD GetSectionVirtualPaddingSize(PIMAGE_SECTION_HEADER pSecHeader);
	PIMAGE_SECTION_HEADER LocateSectionByRawOffset(DWORD dwRawOffset);
	PIMAGE_SECTION_HEADER LocateSectionByRVA(DWORD dwTargetAddr);
	PIMAGE_SECTION_HEADER AddNewSectionToFile(char *szSectionName,DWORD SectionSize);
	PIMAGE_SECTION_HEADER AddNewSectionToMemory(char *szSectionName,DWORD SectionSize);
	PIMAGE_SECTION_HEADER ExtraLastSectionSizeToFile(DWORD SectionAddSize);
	VOID FormatErrorMsg(char *szPrompt, DWORD ErrCode);
	LPSTR GetErrorMsg(char *szBuf,int BufSize);
	CImage();
	virtual ~CImage();

private:
	BOOL VerifyImage(PVOID pBase);
	BOOL SnapThunk(HMODULE hImpMode,char *szImpModeName,PBYTE ImageBase, PIMAGE_THUNK_DATA NameThunk, PIMAGE_THUNK_DATA AddrThunk);
	VOID Cleanup();
	char m_szErrorMsg[1024];
	char m_szPEPath[MAX_PATH];
	
	
};

#endif // !defined(AFX_LOADPE_H__02D71CD1_E8A1_41B5_B185_A841A7F59658__INCLUDED_)
