#include "StdAfx.h"
#include ".\pestructure.h"

#include <ImageHlp.h>
#pragma comment(lib, "ImageHlp")	// 链接到ImageHlp.lib

CPEStructure::CPEStructure(void)
{
	Init();
}

CPEStructure::~CPEStructure(void)
{
	Free();
}
BOOL CPEStructure::OpenFileName(char* FileName)
{
	DWORD dwBytesRead = 0;

	Free();

	hFile=CreateFileA(FileName,
		GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MsgBox("打开文件失败",MB_ICONERROR);
		return FALSE;
	}
	dwFsize=GetFileSize( hFile, 0 );
	if(dwFsize == 0)
	{
		CloseHandle( hFile );
		MsgBox("错误的文件长度",MB_ICONERROR);
		return FALSE;
	}
	strcpy_s(m_FileName,256,FileName);
	dwOutPutSize = dwFsize + PackerCode_Size + ALIGN_CORRECTION;
	pMem=(char*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,dwOutPutSize);
	if(pMem == NULL)
	{
		CloseHandle( hFile );
		MsgBox("申请内存出错",MB_ICONERROR);
		return FALSE;
	}
	ReadFile( hFile, pMem, dwFsize, &dwBytesRead, NULL );
	CloseHandle( hFile );

	CopyMemory(&image_dos_header, pMem, sizeof(IMAGE_DOS_HEADER));

	if( image_dos_header.e_magic != IMAGE_DOS_SIGNATURE )
	{
		MsgBox("不是一个有效的PE文件",MB_ICONERROR);
		return FALSE;
	}
	ReservedHeaderRO= sizeof(IMAGE_DOS_HEADER);

	ReservedHeaderSize= image_dos_header.e_lfanew - sizeof(IMAGE_DOS_HEADER);
	reservedheader=new char[ReservedHeaderSize];

	CopyMemory(&image_nt_headers,
		pMem+image_dos_header.e_lfanew,
		sizeof(IMAGE_NT_HEADERS));
	dwRO_first_section=image_dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS);
	UpdateHeadersSections(TRUE);
	//-------------------------------------------------
	return TRUE;
}
//----------------------------------------------------------------
void CPEStructure::UpdateHeaders(BOOL bSaveAndValidate)
{
	DWORD SectionNum = image_nt_headers.FileHeader.NumberOfSections;

	if( bSaveAndValidate )//TRUE = 保存数据
	{
		CopyMemory(&image_dos_header,pMem,sizeof(IMAGE_DOS_HEADER));
		ReservedHeaderSize=image_dos_header.e_lfanew - sizeof(IMAGE_DOS_HEADER);
		if( ( ReservedHeaderSize & 0x80000000 ) == 0x00000000)
		{
			CopyMemory(reservedheader, pMem+ReservedHeaderRO, ReservedHeaderSize);
		}
		CopyMemory(&image_nt_headers,
			pMem+image_dos_header.e_lfanew,
			sizeof(IMAGE_NT_HEADERS));
		dwRO_first_section = image_dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS);
		CopyMemory(&image_section_header, pMem+dwRO_first_section, SectionNum * sizeof(IMAGE_SECTION_HEADER));
	}
	else				//FALSE = 恢复数据
	{
		CopyMemory(pMem, &image_dos_header, sizeof(IMAGE_DOS_HEADER));
		ReservedHeaderSize=image_dos_header.e_lfanew - sizeof(IMAGE_DOS_HEADER);
		if( (ReservedHeaderSize & 0x80000000) == 0x00000000)
		{
			CopyMemory(pMem+ReservedHeaderRO,reservedheader,ReservedHeaderSize);
		}
		CopyMemory(pMem+image_dos_header.e_lfanew,
			&image_nt_headers,
			sizeof(IMAGE_NT_HEADERS));
		dwRO_first_section=image_dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS);
		CopyMemory(pMem+dwRO_first_section, &image_section_header, SectionNum * sizeof(IMAGE_SECTION_HEADER));
	}
}
//----------------------------------------------------------------
void CPEStructure::UpdateHeadersSections(BOOL bSaveAndValidate)
{
	DWORD i = 0;
	DWORD SectionNum = image_nt_headers.FileHeader.NumberOfSections;
	if(bSaveAndValidate)//TRUE = data is being retrieved
	{
		CopyMemory(&image_dos_header, pMem, sizeof(IMAGE_DOS_HEADER));
		ReservedHeaderSize=image_dos_header.e_lfanew - sizeof(IMAGE_DOS_HEADER);
		if(( ReservedHeaderSize & 0x80000000 ) == 0x00000000)
		{
			CopyMemory(reservedheader, pMem + ReservedHeaderRO, ReservedHeaderSize);
		}
		CopyMemory(&image_nt_headers,
			pMem+image_dos_header.e_lfanew,
			sizeof(IMAGE_NT_HEADERS));
		dwRO_first_section = image_dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS);
		CopyMemory(&image_section_header, pMem + dwRO_first_section, SectionNum * sizeof(IMAGE_SECTION_HEADER));
		for( i = 0; i < SectionNum; i++ )
		{
			image_section[i]=(char*)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,
				PEAlign(image_section_header[i].SizeOfRawData,
				image_nt_headers.OptionalHeader.FileAlignment));
			CopyMemory(image_section[i],
				pMem+image_section_header[i].PointerToRawData,
				image_section_header[i].SizeOfRawData);
		}
		SectionAssigned = TRUE;//已分配空间
	}
	else				//FALSE = data is being initialized 
	{
		CopyMemory(pMem, &image_dos_header, sizeof(IMAGE_DOS_HEADER));
		ReservedHeaderSize = image_dos_header.e_lfanew - sizeof(IMAGE_DOS_HEADER);
		if( ( ReservedHeaderSize & 0x80000000 ) == 0x00000000 )
		{
			CopyMemory(pMem + ReservedHeaderRO, reservedheader, ReservedHeaderSize);
		}
		CopyMemory(pMem+image_dos_header.e_lfanew,
			&image_nt_headers,
			sizeof(IMAGE_NT_HEADERS));
		dwRO_first_section = image_dos_header.e_lfanew + sizeof(IMAGE_NT_HEADERS);
		CopyMemory(pMem + dwRO_first_section, &image_section_header, SectionNum * sizeof(IMAGE_SECTION_HEADER));
		for( i = 0; i < SectionNum; i++ )
		{
			CopyMemory(pMem+image_section_header[i].PointerToRawData,
				image_section[i],
				image_section_header[i].SizeOfRawData);
		}
	}
}
//----------------------------------------------------------------
DWORD CPEStructure::PEAlign(DWORD dwTarNum,DWORD dwAlignTo)
{	
	DWORD dwtemp;
	dwtemp=dwTarNum/dwAlignTo;
	if((dwTarNum%dwAlignTo)!=0)
	{
		dwtemp++;
	}
	dwtemp=dwtemp*dwAlignTo;
	return(dwtemp);
}
//分割得到段和偏移
BOOL CPEStructure::GetFileAddr(char* StrAddress,int* Segment,int* FileOffset)
{
	if( !StrAddress || !Segment || !FileOffset )
		return FALSE;
	char StrSegment[4+1] = {0};
	char StrOffset[8+1] = {0};
	memcpy(StrSegment,StrAddress,4);
	memcpy(StrOffset,StrAddress+5,8);
	*Segment = StringToHex(StrSegment);
	*FileOffset = StringToHex(StrOffset);
	return TRUE;
}

void CPEStructure::LoadMap(char* pmapfilename) 
{
	if( !pmapfilename )
		return;
	FILE* mapfile = NULL;
	mapfile = fopen(pmapfilename,"r+"); //打开MAP FILE处理
	if( !mapfile )
		return;

	FreeMapVector();

	char linestr[512] = {0};            //行数据
	int  readcnt = 0;                   //读的行数
	BOOL bBegin = FALSE;                //开始
	while(!feof(mapfile))
	{
		memset(linestr,0,512);
		if(fgets(linestr,512,mapfile) == NULL) //读一行，最多读512个字节
		{
			break;
		}
		readcnt++; //计算读了多少行
		if( strstr(linestr,"entry point at") != NULL )
		{
			continue;
		}
		if( strstr(linestr,"Line numbers for") != NULL )//行号就不看了
		{
			break;
		}
		if( ( strstr(linestr,"Publics by Value") != NULL && strstr(linestr,"Rva+Base") != NULL ) ||
			( strstr(linestr,"Static symbols") != NULL ) )//开始搜索符号
		{
			bBegin = TRUE;
			continue;
		}
		char StrLine[512] = {0};
		if( !Trim(linestr,StrLine) )//是空行读下一行
		{
			continue;
		}
		if( bBegin )//开始处理
		{
			char StrAddress[16] = {0};//段:偏移
			char StrSymbol[512] = {0};//符号名
			char StrVirtualAddr[16] = {0};//线性地址
			char StrLibObject[512] = {0};//输入文件名
			int Segment = 0;
			int FileOffset = 0;

			memcpy(StrAddress,StrLine,13);
			int nstrlen = strlen(StrLine);
			memmove(StrLine,StrLine+13,nstrlen - 13 + 1);//跳过段偏移

			GetFileAddr(StrAddress,&Segment,&FileOffset);//得到段和偏移
			if( Segment == 0 )//段为0不算
				continue;

			Trim(StrLine,StrLine);
			strcpy_s(StrSymbol,512,StrLine);
			*strchr(StrSymbol,' ') = '\0';                          
			int nlen = strlen(StrSymbol);
			nstrlen = strlen(StrLine);
			memmove(StrLine,StrLine+nlen,strlen(StrLine)-nlen+1);//跳过符号名

			Trim(StrLine,StrLine);

			memcpy(StrVirtualAddr,StrLine,strchr(StrLine,' ')-StrLine);

			nstrlen = strlen(StrLine);
			memmove(StrLine,StrLine+8,nstrlen-8+1);//跳过VirtualAddress

			nstrlen = strlen(StrLine);
			memmove(StrLine,StrLine+5,nstrlen-5+1);//跳过f i ...
			strcpy(StrLibObject,StrLine);
			MapStructrue *mapstruct = new MapStructrue;
			mapstruct->Segment = Segment;                            //段
			mapstruct->Offset = FileOffset;                          //偏移
			strcpy(mapstruct->SymbleName,StrSymbol);                 //符号名
			mapstruct->VirtualAddress = StringToHex(StrVirtualAddr); //线性地址
			strcpy(mapstruct->LibObject,StrLibObject);               //输入文件名

			MapVector.push_back(mapstruct);
		}
	}

	fclose(mapfile);
	mapfile = NULL;

	for( vector<MapStructrue*>::iterator itr = MapVector.begin(); itr != MapVector.end(); itr++ )
	{
		MapStructrue *stu = *itr;
		int a = stu->Segment;
		int b = stu->Offset;
		int nlen = strlen("__ehhandler$");
		if( strstr(stu->SymbleName,"__ehhandler$") )//去掉标记
			memmove(stu->SymbleName,stu->SymbleName+nlen,strlen(stu->SymbleName) - nlen + 1);

		if (!UnDecorateSymbolName(stu->SymbleName, stu->SymbleName, 512, 0xFFF))//no UNDNAME_COMPLETE
		{
			// UnDecorateSymbolName failed
			DWORD error = GetLastError();
			char strerr[255] = {0};
			sprintf(strerr,"解析符号名时出现错误 %d\n", error);
			MsgBox(strerr,MB_ICONERROR);
			break;
		}
		//OutPutStr("%04X:%08X %s %08X %s\n",stu->Segment,stu->Offset,stu->SymbleName,stu->VirtualAddress,stu->LibObject);
		OutPutStr("%s %08X %s\n",stu->SymbleName,stu->VirtualAddress,stu->LibObject);
	}
}

MapStructrue* CPEStructure::GetMap(char* funcname)
{
	for( vector<MapStructrue*>::iterator itr = MapVector.begin(); itr != MapVector.end(); itr++ )
	{
		MapStructrue *stu = *itr;
		if( _stricmp(stu->SymbleName,funcname) == 0)
			return stu;
	}
	return NULL;
}
DWORD CPEStructure::GetNewSection()
{
	DWORD SectionNum = image_nt_headers.FileHeader.NumberOfSections;//得到节数
	return PEAlign( image_section_header[SectionNum-1].VirtualAddress + image_section_header[SectionNum-1].Misc.VirtualSize,
			image_nt_headers.OptionalHeader.SectionAlignment);
}
//----------------------------------------------------------------
// return values:
// 0 - no room for a new section
// 1 - file already encrypted
// else: returns a pointer to the IMAGE_SECTION_HEADER struct of the new section
PIMAGE_SECTION_HEADER CPEStructure::AddSection(char* Base,int len,char* SectionName)
{
	DWORD newSectionOffset;
	DWORD SectionNum = image_nt_headers.FileHeader.NumberOfSections;//得到节数
	newSectionOffset = dwRO_first_section + image_nt_headers.FileHeader.NumberOfSections*sizeof(IMAGE_SECTION_HEADER);

	// check whether there's room for a new section
	if( image_nt_headers.OptionalHeader.SizeOfHeaders < ( newSectionOffset + sizeof(IMAGE_SECTION_HEADER) ) )
	{
		return NULL;
	}

	// create a new section

	// go to the last section
	for(DWORD i=0;i<(SectionNum-1);i++)
	{
		image_section_header[i].Characteristics |= IMAGE_SCN_MEM_WRITE;//可写属性
	}

	// 开始建立一个新节
	CopyMemory(&image_section_header[SectionNum],&image_section_header[SectionNum-1],sizeof(IMAGE_SECTION_HEADER));

	//新节的虚拟地址
	image_section_header[SectionNum].VirtualAddress = PEAlign(image_section_header[SectionNum-1].VirtualAddress+
		image_section_header[SectionNum-1].Misc.VirtualSize,image_nt_headers.OptionalHeader.SectionAlignment);

	image_section_header[SectionNum].Misc.VirtualSize = len;//0x4000;//段长度

	// RawSize..
	//image_section_header[SectionNum].SizeOfRawData = image_section_header[SectionNum-1].PointerToRawData + image_section_header[SectionNum-1].SizeOfRawData;//bughoho
	image_section_header[SectionNum].SizeOfRawData = PEAlign(image_section_header[SectionNum].Misc.VirtualSize,
													 image_nt_headers.OptionalHeader.FileAlignment);//bughoho
	dwOutPutSize += image_section_header[SectionNum].SizeOfRawData;
	// Section name
	int l=(int)strlen(SectionName);
	FillMemory(image_section_header[SectionNum].Name,8,0x00);
	CopyMemory(image_section_header[SectionNum].Name,SectionName,l);
	
	// Characteristics
	image_section_header[SectionNum].Characteristics = IMAGE_SCN_MEM_EXECUTE |
													IMAGE_SCN_MEM_WRITE | 
													IMAGE_SCN_MEM_READ |
													IMAGE_SCN_MEM_EXECUTE |
													IMAGE_SCN_CNT_UNINITIALIZED_DATA |
													IMAGE_SCN_CNT_INITIALIZED_DATA |
													IMAGE_SCN_CNT_CODE;

	// RawOffset
	image_section_header[SectionNum].PointerToRawData = PEAlign(image_section_header[SectionNum-1].PointerToRawData
														+ image_section_header[SectionNum-1].SizeOfRawData,
														image_nt_headers.OptionalHeader.FileAlignment);
	// update the PE header
	image_nt_headers.FileHeader.NumberOfSections++;
	// newsection -> will be returned

	image_nt_headers.OptionalHeader.SizeOfImage = image_section_header[SectionNum].VirtualAddress +
												  image_section_header[SectionNum].Misc.VirtualSize;

	memcpy(pMem + image_section_header[SectionNum].PointerToRawData,Base,len);
	return ((PIMAGE_SECTION_HEADER)&image_section_header[SectionNum]);
}
//----------------------------------------------------------------
void CPEStructure::Init()
{
	SectionAssigned       = FALSE;
	reservedheader        = NULL;
	ReservedHeaderSize    = 0;
	ReservedHeaderRO      = 0;
	hFile                 = 0;
	dwFsize               = 0;
	dwOutPutSize          = 0;
	memset(m_FileName,0,255);
	memset(m_MapFileName,0,255);
}

void CPEStructure::FreeMapVector()
{
	for( vector<MapStructrue*>::iterator itr = MapVector.begin(); itr != MapVector.end(); itr++ )
	{
		MapStructrue* stu = *itr;
		if( stu )
		{
			delete stu;
		}
	}
	MapVector.clear();
}
//----------------------------------------------------------------
void CPEStructure::Free()
{
	if( reservedheader )
	{
		delete[] reservedheader;
		reservedheader = NULL;
	}
	if( !SectionAssigned )
		return;
	DWORD SectionNum = image_nt_headers.FileHeader.NumberOfSections;
	for(DWORD i = 0 ;i < SectionNum;i++)
	{
		GlobalFree(image_section[i]);
		image_section[i] = NULL;
	}
	Init();
}
//制造一个PE
void CPEStructure::MakePE(char* filename,int len)
{
	HANDLE handle;
	DWORD Num;
	handle= ::CreateFileA(filename,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE== handle )
	{
		MessageBox(0,"打开文件失败!","错误",MB_OK);
		return;
	}
	::SetFilePointer(handle,0,0,FILE_BEGIN);
	::WriteFile(handle,pMem,dwOutPutSize,&Num,NULL);
	::CloseHandle(handle);
}  