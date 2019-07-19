/*******************************************************
《加密与解密》第三版配套实例
15.8.2 编写静态脱壳器                                    
  unaspack10804.cpp : Defines the entry point for the console application.
  code by DiKen
(c)  看雪软件安全网站 www.pediy.com 2000-2008
********************************************************/


#include <windows.h>
#include <stdio.h>


#define halt0(msg, result)							\
	printf(msg);									\
	return result;									\

#define halt1(msg, result)							\
	CloseHandle(hFile);								\
	printf(msg);									\
	return result;									\

#define halt2(msg, result)							\
	CloseHandle(hMap);								\
	CloseHandle(hFile);								\
	printf(msg);									\
	return result;									\

#define halt3(msg, result)							\
	UnmapViewOfFile(pMap);							\
	CloseHandle(hMap);								\
	CloseHandle(hFile);								\
	printf(msg);									\
	return result;									\


#define halt4(msg, result)							\
	VirtualFree(Image, 0, MEM_RELEASE);				\
	UnmapViewOfFile(pMap);							\
	CloseHandle(hMap);								\
	CloseHandle(hFile);								\
	printf(msg);									\
	return result;									\

#define halt5(msg, result)							\
	VirtualFree(Image, 0, MEM_RELEASE);				\
	VirtualFree(pLoaderCore, 0, MEM_RELEASE);		\
	UnmapViewOfFile(pMap);							\
	CloseHandle(hMap);								\
	CloseHandle(hFile);								\
	printf(msg);									\
	return result;									\
  	
#define halt6(msg, result)							\
	CloseHandle(hFileOutput);						\
	VirtualFree(Image, 0, MEM_RELEASE);				\
	VirtualFree(pLoaderCore, 0, MEM_RELEASE);		\
	UnmapViewOfFile(pMap);							\
	CloseHandle(hMap);								\
	CloseHandle(hFile);								\
	printf(msg);									\
	return result;									\
  			
		

DWORD AlignUp(DWORD n, DWORD A)
{
	return ((n / A) +  ((n % A) ? 1 : 0)) * A;
}

int IndexOfRva(void *pMap, DWORD Rva)
{
	PIMAGE_DOS_HEADER pd = (PIMAGE_DOS_HEADER)pMap;
	PIMAGE_NT_HEADERS ph = (PIMAGE_NT_HEADERS)((BYTE *)pMap + pd->e_lfanew);
	PIMAGE_SECTION_HEADER ps = (PIMAGE_SECTION_HEADER)((BYTE *)ph + sizeof(IMAGE_NT_HEADERS));

	if (Rva > ph->OptionalHeader.SizeOfImage)
	{
		return -2;
	};


	if (ps->VirtualAddress > Rva)
	{
		return -1;
	}
	else
	{
		for (int i = 0; i < ph->FileHeader.NumberOfSections; i++)
		{
			if ((ps->VirtualAddress <= Rva) && ((ps->VirtualAddress + AlignUp(ps->Misc.VirtualSize ,ph->OptionalHeader.SectionAlignment)) > Rva))
			{
				return i;
			};
			ps++;
		}
	}
	return -2;
}

void *RvaToPointer(void *pMap, DWORD Rva)
{
	int i = IndexOfRva(pMap, Rva);
	if (i < 0)
	{
		if (i = -2) return NULL;
		return ((BYTE *)pMap + Rva);
	}
	else
	{
		PIMAGE_DOS_HEADER pd = (PIMAGE_DOS_HEADER)pMap;
		PIMAGE_NT_HEADERS ph = (PIMAGE_NT_HEADERS)((BYTE *)pMap + pd->e_lfanew);
		PIMAGE_SECTION_HEADER ps = (PIMAGE_SECTION_HEADER)((BYTE *)ph + sizeof(IMAGE_NT_HEADERS));
		ps += i;
		return ((BYTE *)pMap + ps->PointerToRawData + Rva - ps->VirtualAddress);				
	};
}


#define getbit()\
	(b = b * 2, b & 0xFF ? ((b >> 8) & 1) : (((b = ((*pSrc++ * 2) + ((b >> 8) & 1))) >> 8) & 1))


#define CopyRepeat(dst, off, len)\
	for(;len > 0; len--)\
{\
	*dst = *(dst - off);\
	dst++;\
}

int aPLibDePack(void *Src, void *Dst)
{
	BYTE *pSrc = (BYTE *)Src;
	BYTE *pDst = (BYTE *)Dst; 
	DWORD off, last_off = 0;
	DWORD len;
	DWORD b = 0x80;

	*pDst++ = *pSrc++;
	for(;;)
	{
		///////////////////////
		//最高bit为0则拷贝1个字符; 否则
		if (!getbit())
		{
			*pDst++ = *pSrc++;
			continue;
		};

		//
		if (getbit())
		{
			off = 0;
			if (getbit())
			{
				//重复
				off = off * 2 + getbit();
				off = off * 2 + getbit();
				off = off * 2 + getbit();
				off = off * 2 + getbit();
				if (off != 0)
				{
					*pDst = *(pDst - off);
					pDst++;
				}
				else
				{
					*pDst++ = 0;
				};
				continue;
			}//goto case 5
		}
		else
		{
			//case1
			off = 1;

			do 
			{
				off = off * 2 + getbit();
			}while(getbit());

			off = off - 2;
			if (off == 0)
			{
				len = 1;
				do
				{
					len = len * 2 + getbit();
				}while (getbit());
				CopyRepeat(pDst, last_off, len);      
			}
			else
			{
				off--;
				off = off * 256 + *pSrc++;
				last_off = off;

				len = 1;
				do 
				{
					len = len * 2 + getbit();
				}while (getbit());

				//重复次数
				//[0-7F]                + 2
				//(7F-0x500)            + 0
				//[0x500, 0x7D00)       + 1
				//[7D00-]               + 2
				off = off - 0x80;				
				if (off >= 0x500 - 0x80) len++;
				if (off >= 0x7D00 - 0x80) len++;      
				off = off + 0x80;
				CopyRepeat(pDst, off, len);				
			};
			continue;
		};  


		///////////////////
		//case5
		off = *pSrc++;
		len = 2 + (off & 1);
		off = off >> 1;  
		if (off == 0) break;
		last_off = off;

		CopyRepeat(pDst, off, len);
	};
	return (pDst - (BYTE *)Dst);
}



int main(int argc, char* argv[])
{

	char szFileName[MAX_PATH] = "D:\\calc.exe.bak.exe";
	//输入文件变量
	HANDLE hFile;
	DWORD dwFileSize;
	HANDLE hMap;
	void *pMap;

	




	//////////////////////////////////////////////////////////////////////////
	//打开输入文件， 并Map到内存
	hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		halt0("error open file for read!\n", -1);
	};

	dwFileSize = GetFileSize(hFile, NULL);


	//OpenMap输入文件
	hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY | SEC_COMMIT, 0, 0, NULL);
	if (hMap == NULL)
	{
		halt1("error open file map  for read!\n", -2);
	};
	//MapFile
	pMap = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	if (pMap == NULL)
	{
		halt2("error map file for read!\n", -3);
	};

	//////////////////////////////////////////////////////////////////////////
	//开始检测文件了

	//DOS头检测
	if (dwFileSize < sizeof(IMAGE_DOS_HEADER))
	{
		halt3("file size small\n", -4);
	};

	PIMAGE_DOS_HEADER pd = (PIMAGE_DOS_HEADER)pMap;
	if (pd->e_magic != IMAGE_DOS_SIGNATURE)
	{
		halt3("file not pe -> dos magic\n", -5);
	};

	//PE头检测
	DWORD dwProbeSize = pd->e_lfanew + sizeof(IMAGE_NT_HEADERS);
    if (dwFileSize < dwProbeSize)
	{
		halt3("file size small 2\n", -6);
	};
	PIMAGE_NT_HEADERS ph = (PIMAGE_NT_HEADERS)((BYTE *)pMap + pd->e_lfanew);
	if (ph->Signature != IMAGE_NT_SIGNATURE)
	{		
		halt3("file not pe -> pe magic\n", -7);
	};

	//我们只支持PE32,不支持PE32+啦
	if (ph->OptionalHeader.Magic == 0x20B)
	{
		halt3("file not pe32\n", -8);
	};

	//PE Section映射
	dwProbeSize += ph->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER); 
	if (dwFileSize < dwProbeSize)
	{		
		halt3("file size small 3\n", -9);
	};

	PIMAGE_SECTION_HEADER psbase = (PIMAGE_SECTION_HEADER)((BYTE *)ph + sizeof(IMAGE_NT_HEADERS));//得到区块表的地址
	
	//文件大小检测
	//有问题，循环检测没啥意义
	PIMAGE_SECTION_HEADER ps = psbase;
	dwProbeSize = 0;	
	for (int i = 0; i < ph->FileHeader.NumberOfSections; i++)
	{
		if (ps->PointerToRawData >= dwProbeSize) 
			dwProbeSize = ps->PointerToRawData + ps->SizeOfRawData;
	};	
	if (dwFileSize < dwProbeSize)
	{
		//节数据读不到了	
		halt3("file size small 4\n", -10);
	};
	
	//Overlay计算
	



	//////////////////////////////////////////////////////////////////////////
	//壳检测
	//如果不是这个版本的壳就退出啦

	//因为我们只针对特定壳，所以就直接用绝对偏移来比较，如果不是就认为不是这个壳啦
	//当然只要稍微加点PE-PATCH的话也就认不出来了
	DWORD dwEntrypoint = ph->OptionalHeader.AddressOfEntryPoint;
	BYTE *pEntrypoint = (BYTE *)RvaToPointer(pMap, dwEntrypoint);
	
	//根据入口指令判断是否是该壳
	if ((*pEntrypoint != 0x60)
		||(*((DWORD*)(pEntrypoint + 0x49)) != 0x442914BB)//mov ebx, 442914
		||(*((DWORD*)(pEntrypoint + 0xBA)) != 0x54A)	//push 54Ah
		||(*((DWORD*)(pEntrypoint + 0x29)) != dwEntrypoint)	//dd Entrypoint??
	)
	{
		halt3("not aspack 1.08.04", -11);
	};

	//////////////////////////////////////////////////////////////////////////
	//现在可以工作了
	//映射物理文件到内存
	char *Image;
	Image = (char *)VirtualAlloc(NULL, AlignUp(ph->OptionalHeader.SizeOfImage, ph->OptionalHeader.SectionAlignment), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (Image == NULL)
	{
		halt3("cannot map file", -12);
	};	
	ps = psbase;
	memcpy(Image, pMap, ph->OptionalHeader.SizeOfHeaders);
	for (i = 0; i < ph->FileHeader.NumberOfSections; i++)
	{
		memcpy(Image + ps->VirtualAddress,
			    (char *)pMap + ps->PointerToRawData, AlignUp(ps->SizeOfRawData, ph->OptionalHeader.FileAlignment));
		ps++;
	};


	//1.解密关键数据, 这部分
	BYTE *pLoaderCore = (BYTE *) VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (pLoaderCore == NULL)
	{
		halt4("alloc virtual memory fail", -13);
	};

	//010270FD - 01027000
	DWORD LoaderSize = aPLibDePack(pEntrypoint + 0xFD, pLoaderCore);
	if (LoaderSize == 0)
	{
		halt5("loader decode failed.", -14);
	};

	//+08
	DWORD offImageBase = 0x0C;
	//+10
	//+14
	DWORD offReloc	=	0x18;//+18
	DWORD offImport	=	0x1C;
	DWORD offEntrypoint	=	0x20;
	//
	DWORD offSectionInfo	=	0x2C;





	
	//为了防止可能有些节没有压缩, 因此, 直接采用Map形式


	//读取数据开始进行解码
	struct stSectionInfo 
	{
		DWORD Rva;
		DWORD VSize;	
	};


	stSectionInfo *pSectionInfo = (stSectionInfo *)(pLoaderCore + offSectionInfo);
	bool isfirst = true;
	while (pSectionInfo->Rva != 0)
	{
		//void *WorkMem = VirtualAlloc(NULL, pSectionInfo->VSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		//直接解压到Image
		DWORD dwRealSize = aPLibDePack(RvaToPointer(pMap, pSectionInfo->Rva), Image + pSectionInfo->Rva);

		//如果是第一次需要修复E8E9
		if (isfirst)
		{
			isfirst = false;
			BYTE *p = (BYTE *)Image + pSectionInfo->Rva;
			int nSize = dwRealSize - 6;
			DWORD off = 0;
			while (nSize > 0)
			{
				if ((*p == 0xE8)||(*p == 0xE9))
				{
					*(DWORD *)(p + 1) -= off;
					off += 4;
					p += 4;
					nSize -= 4;					
				};
				off++;
				p++;
				nSize--;
			}
		};
		pSectionInfo++;
	};


	//////////////////////////////////////////////////////////////////////////
	//修正Image里面的PE头
    pd = (PIMAGE_DOS_HEADER)Image;
	ph = (PIMAGE_NT_HEADERS)((BYTE *)Image + pd->e_lfanew);
	psbase = (PIMAGE_SECTION_HEADER)((BYTE *)ph + sizeof(IMAGE_NT_HEADERS));

	//文件对齐方式
	ph->OptionalHeader.SizeOfImage = AlignUp(ph->OptionalHeader.SizeOfImage, ph->OptionalHeader.SectionAlignment);
	ph->OptionalHeader.FileAlignment = ph->OptionalHeader.SectionAlignment;
	ph->OptionalHeader.SizeOfHeaders = psbase->VirtualAddress;

	//节对齐
	ps = psbase;
	for (i = 0; i < ph->FileHeader.NumberOfSections; i++)
	{		
		ps->SizeOfRawData = ps->Misc.VirtualSize = AlignUp(ps->Misc.VirtualSize, ph->OptionalHeader.SectionAlignment);		
		ps->PointerToRawData = ps->VirtualAddress;
		ps++;
	}

	//////////////////////////////////////////////////////////////////////////
	//重定位处理
	//由于壳没有破坏Reloc, 因此, 我们直接将Reloc的Rva写回PE头就OK了
	DWORD dwRvaReloc = *(DWORD *)(pLoaderCore + offReloc);
	if (dwRvaReloc != 0)
	{
		//计算Reloc Size
		DWORD RelocSize = 0;
		BYTE *pReloc = (BYTE *)Image + dwRvaReloc;

		while (*(DWORD*)pReloc != 0)
		{
			RelocSize += *(DWORD*)(pReloc + 4);
			pReloc += *(DWORD*)(pReloc + 4);
		};
		
		ph->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = RelocSize;
		ph->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = dwRvaReloc;
	};


	//////////////////////////////////////////////////////////////////////////
	//IAT处理
	//由于壳没有破坏IAT, 我们简单的将IAT的地址写回PE头
	DWORD dwImportRva = *(DWORD *)(pLoaderCore + offImport);
	if (dwImportRva != 0)
	{
		DWORD ImportSize = sizeof(IMAGE_IMPORT_DESCRIPTOR);
		PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(Image + dwImportRva);
		while (pImport->Name != 0)
		{
			ImportSize += sizeof(IMAGE_IMPORT_DESCRIPTOR);
			pImport++;
		};

		ph->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = ImportSize;
		ph->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = dwImportRva;
	}



	//////////////////////////////////////////////////////////////////////////
	//OriginalEntrypoint处理
	//由于壳没有抽取入口, 因此我们只是简单的将OEP写回PE头
	DWORD dwEntrypointRva = *(DWORD *)(pLoaderCore + offEntrypoint);
	ph->OptionalHeader.AddressOfEntryPoint = dwEntrypointRva;

	

	//////////////////////////////////////////////////////////////////////////
	//
	//保存文件
	//
	//////////////////////////////////////////////////////////////////////////
	strcat(szFileName,"_.EXE");

	HANDLE hFileOutput = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (hFileOutput == INVALID_HANDLE_VALUE)
	{
		halt5("create output file failed.", -15);
	};

	DWORD dwWrittenBytes;
	if (!WriteFile(hFileOutput, Image, ph->OptionalHeader.SizeOfImage, &dwWrittenBytes, NULL))
	{
		halt6("write output file error 1\n", -16);
	};




	//////////////////////////////////////////////////////////////////////////
	//写附加数据



	//////////////////////////////////////////////////////////////////////////
	//脱壳成功,	关闭文件

	halt6("unpack success\n", 0);

    
	//////////////////////////////////////////////////////////////////////////
	//
	//这中间可以进行优化, 比如FileAlign, 资源重构等
	//当然如果修复的好固然是好，但可能会优化之后根本无法运行啦
	//
	//为什么没有重构, 简单的说, PE结构其实很复杂, 重构的时候可能会有问题.
	//
	//////////////////////////////////////////////////////////////////////////
}


