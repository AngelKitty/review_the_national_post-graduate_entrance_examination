/*-----------------------------------------------------------------------
第19章  外壳编写基础
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include "stdafx.h"
#include "PE.h"


CPE::CPE() :m_dwFileSize(0), m_pFileBase(NULL),
m_dwFileAlign(0), m_dwMemAlign(0)
{
}


CPE::~CPE()
{
}

DWORD CPE::RVA2OffSet(DWORD dwRVA, PIMAGE_NT_HEADERS64  pNt)
{
	DWORD dwOffset = 0;
	// 1. 获取第一个区段结构体
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	// 2. 获取区段数量
	DWORD dwSectionCount = pNt->FileHeader.NumberOfSections;
	// 3. 遍历区段信息表
	for (DWORD i = 0; i < dwSectionCount; i++)
	{
		// 4. 匹配RVA所在的区段
		if (dwRVA >= pSection[i].VirtualAddress &&
			dwRVA < (pSection[i].VirtualAddress + pSection[i].Misc.VirtualSize)
			)
		{   // 计算对应的文件偏移
			dwOffset = dwRVA - pSection[i].VirtualAddress +
				pSection[i].PointerToRawData;
			return dwOffset;
		}
	}
	return dwOffset;
}


BOOL CPE::InitPE(CString strPath)
{
	if (m_objFile.m_hFile == INVALID_HANDLE_VALUE && m_pFileBase) {
		m_objFile.Close(); delete m_pFileBase; m_pFileBase = NULL;
	}

	m_objFile.Open(strPath, CFile::modeRead);
	m_dwFileSize = (DWORD)m_objFile.GetLength();
	m_pFileBase = new BYTE[m_dwFileSize];
	if (m_objFile.Read(m_pFileBase, (DWORD)m_dwFileSize))
	{
		// 1. 获取DOS头
		PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pFileBase;
		// 2. 获取NT头
		m_pNT = (PIMAGE_NT_HEADERS64)((ULONGLONG)m_pFileBase + pDos->e_lfanew);
		// 3. 获取文件对齐、内存对齐等信息
		m_dwFileAlign = m_pNT->OptionalHeader.FileAlignment;
		m_dwMemAlign = m_pNT->OptionalHeader.SectionAlignment;
		m_dwImageBase = m_pNT->OptionalHeader.ImageBase;
		m_dwOEP = m_pNT->OptionalHeader.AddressOfEntryPoint;
		m_dwCodeBase = m_pNT->OptionalHeader.BaseOfCode;
		m_dwCodeSize = m_pNT->OptionalHeader.SizeOfCode;


		// 3. 获取最后一个区段后的地址
		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(m_pNT);
		m_pLastSection = &pSection[m_pNT->FileHeader.NumberOfSections];

		// 4. 获取新加区段的起始RVA,  内存4K对齐
		DWORD dwVirtualSize = m_pLastSection[-1].Misc.VirtualSize;
		if (dwVirtualSize%m_dwMemAlign)
			dwVirtualSize = (dwVirtualSize / m_dwMemAlign + 1) * m_dwMemAlign;
		else
			dwVirtualSize = (dwVirtualSize / m_dwMemAlign)* m_dwMemAlign;
		m_dwNewSectionRVA = m_pLastSection[-1].VirtualAddress + dwVirtualSize;

		return TRUE;
	}
	return FALSE;
}

ULONGLONG CPE::AddSection(LPBYTE pBuffer, DWORD dwSize, PCHAR pszSectionName)
{
	// 1 修改文件头中的区段数量
	m_pNT->FileHeader.NumberOfSections++;

	// 2 增加区段表项
	memset(m_pLastSection, 0, sizeof(IMAGE_SECTION_HEADER));
	strcpy_s((char*)m_pLastSection->Name, IMAGE_SIZEOF_SHORT_NAME, pszSectionName);

	DWORD dwVirtualSize = 0; // 区段虚拟大小
	DWORD dwSizeOfRawData = 0; // 区段文件大小
	DWORD dwSizeOfImage = m_pNT->OptionalHeader.SizeOfImage;
	{
		if (dwSizeOfImage%m_dwMemAlign)
			dwSizeOfImage = (dwSizeOfImage / m_dwMemAlign + 1) * m_dwMemAlign;
		else
			dwSizeOfImage = (dwSizeOfImage / m_dwMemAlign) * m_dwMemAlign;

		if (dwSize%m_dwMemAlign)
			dwVirtualSize = (dwSize / m_dwMemAlign + 1) * m_dwMemAlign;
		else
			dwVirtualSize = (dwSize / m_dwMemAlign) * m_dwMemAlign;

		if (dwSize%m_dwFileAlign)
			dwSizeOfRawData = (dwSize / m_dwFileAlign + 1) * m_dwFileAlign;
		else
			dwSizeOfRawData = (dwSize / m_dwFileAlign) * m_dwFileAlign;
	}

	m_pLastSection->VirtualAddress = dwSizeOfImage;
	m_pLastSection->PointerToRawData = m_dwFileSize;
	m_pLastSection->SizeOfRawData = dwSizeOfRawData;
	m_pLastSection->Misc.VirtualSize = dwVirtualSize;
	m_pLastSection->Characteristics = 0XE0000040;

	// 3 增加文件大小,创建文件，添加代码
	m_pNT->OptionalHeader.SizeOfImage = dwSizeOfImage + dwVirtualSize;
	m_pNT->OptionalHeader.AddressOfEntryPoint = m_dwNewOEP + m_pLastSection->VirtualAddress;

	// 3.1 生成输出文件路径
	CString strPath = m_objFile.GetFilePath();
	TCHAR szOutPath[MAX_PATH] = { 0 };
	LPWSTR strSuffix = PathFindExtension(strPath);                     // 获取文件的后缀名
	wcsncpy_s(szOutPath, MAX_PATH, strPath, wcslen(strPath)); // 备份目标文件路径到szOutPath
	PathRemoveExtension(szOutPath);                                         // 将szOutPath中保存路径的后缀名去掉
	wcscat_s(szOutPath, MAX_PATH, L"_15Pack");                       // 在路径最后附加“_15Pack”
	wcscat_s(szOutPath, MAX_PATH, strSuffix);                           // 在路径最后附加刚刚保存的后缀名

	// 3.2 创建文件
	CFile objFile(szOutPath, CFile::modeCreate | CFile::modeReadWrite);
	objFile.Write(m_pFileBase, m_dwFileSize);
	objFile.SeekToEnd();
	objFile.Write(pBuffer, dwSize);

	return m_pLastSection->VirtualAddress;
}

// 在内存中重定位Stub
void CPE::FixReloc(PBYTE lpImage, PBYTE lpCode, DWORD dwCodeRVA)
{
	// 1. 获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpImage;
	// 2. 获取NT头
	PIMAGE_NT_HEADERS64  pNt = (PIMAGE_NT_HEADERS64)((ULONGLONG)lpImage + pDos->e_lfanew);
	// 3. 获取数据目录表
	PIMAGE_DATA_DIRECTORY pRelocDir = pNt->OptionalHeader.DataDirectory;
	pRelocDir = &(pRelocDir[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	// 4. 获取重定位目录
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)((ULONGLONG)lpImage + pRelocDir->VirtualAddress);
	typedef struct {
		WORD Offset : 12;          // (1) 大小为12Bit的重定位偏移
		WORD Type : 4;             // (2) 大小为4Bit的重定位信息类型值
	}TypeOffset, *PTypeOffset;	   // 这个结构体是A1Pass总结的

	// 循环获取每一个MAGE_BASE_RELOCATION结构的重定位信息
	while (pReloc->VirtualAddress)
	{
		PTypeOffset pTypeOffset = (PTypeOffset)(pReloc + 1);
		ULONGLONG dwSize = sizeof(IMAGE_BASE_RELOCATION);
		ULONGLONG dwCount = (pReloc->SizeOfBlock - dwSize) / 2;
		for (ULONGLONG i = 0; i < dwCount; i++)
		{
			if (*(PULONGLONG)(&pTypeOffset[i]) == NULL)
				break;
			ULONGLONG dwRVA = pReloc->VirtualAddress + pTypeOffset[i].Offset;
			PULONGLONG pRelocAddr = (PULONGLONG)((ULONGLONG)lpImage + dwRVA);
			// 修复重定位信息   公式：需要修复的地址-原映像基址-原区段基址+现区段基址+现映像基址
			ULONGLONG dwRelocCode = *pRelocAddr - pNt->OptionalHeader.ImageBase - pNt->OptionalHeader.BaseOfCode +
				                                           dwCodeRVA + m_dwImageBase;
			*pRelocAddr = dwRelocCode;
		}
		pReloc = (PIMAGE_BASE_RELOCATION)((ULONGLONG)pReloc + pReloc->SizeOfBlock);
	}
}

void CPE::SetNewOEP(DWORD dwOEP)
{
	m_dwNewOEP = dwOEP;
}

void CPE::ClearBundleImport()
{
	// 1. 获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pFileBase;
	// 2. 获取NT头
	PIMAGE_NT_HEADERS64  pNt = (PIMAGE_NT_HEADERS64)((ULONGLONG)m_pFileBase + pDos->e_lfanew);
	// 3. 获取数据目录表
	PIMAGE_DATA_DIRECTORY pDir = pNt->OptionalHeader.DataDirectory;
	// 4. 清空绑定输入表项
	ZeroMemory(&(pDir[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT]), sizeof(IMAGE_DATA_DIRECTORY));
}

void CPE::ClearRandBase()
{
	m_pNT->OptionalHeader.DllCharacteristics = 0;
}

DWORD CPE::GetSectionData(PBYTE lpImage, DWORD dwSectionIndex, PBYTE& lpBuffer, DWORD& dwCodeBaseRVA)
{
	// 1. 获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpImage;
	// 2. 获取NT头
	PIMAGE_NT_HEADERS64  pNt = (PIMAGE_NT_HEADERS64)((ULONGLONG)lpImage + pDos->e_lfanew);
	// 3. 获取第一个区段结构体
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	DWORD dwSize = pSection[0].SizeOfRawData;

	ULONGLONG dwCodeAddr = (ULONGLONG)lpImage + pSection[0].VirtualAddress;
	lpBuffer = (PBYTE)dwCodeAddr;
	dwCodeBaseRVA = pSection[0].VirtualAddress;
	return dwSize;
}


DWORD CPE::XorCode(BYTE byXOR)
{
	DWORD dwVirtualAddr = m_dwCodeBase;
	DWORD dwOffset = RVA2OffSet(m_dwCodeBase, m_pNT);
	if (!dwOffset)
	{// 兼容 debug版本，第一个区段没有大小
		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(m_pNT);
		dwOffset = RVA2OffSet(pSection[1].VirtualAddress, m_pNT);
		dwVirtualAddr = pSection[1].VirtualAddress;
	}
	PBYTE pBase = (PBYTE)((ULONGLONG)m_pFileBase + dwOffset);

	for (DWORD i = 0; i < m_dwCodeSize; i++)
	{
		pBase[i] ^= byXOR;
	}

	return dwVirtualAddr;
}