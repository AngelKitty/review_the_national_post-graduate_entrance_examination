#pragma once
#include "comm.h"

#include <math.h>

#include <vector>
using namespace std;

#define MAX_SECTION_NUM 20
const DWORD PackerCode_Size = 0;//壳代码最大长度
const DWORD ALIGN_CORRECTION = 0;//文件对齐

class CPEStructure
{
public:
	CPEStructure(void);
	~CPEStructure(void);
private:
	DWORD   ReservedHeaderSize;//保留头长度
	DWORD   ReservedHeaderRO;  //DOS头长度

	BOOL    SectionAssigned;//是否已分配
	HANDLE  hFile;//PE文件句柄
	DWORD   dwFsize;//PE文件长度
	DWORD   dwOutPutSize;//PE文件输出长度
	char    *pMem;//PE文件缓冲区
private:
	char    m_FileName[256];//文件名
	char    m_MapFileName[256];//map文件名
public://映射文件
	vector<MapStructrue*> MapVector;
	void LoadMap(char* pmapfilename);//载入分析map文件
	BOOL GetFileAddr(char* StrAddress,int* Segment,int* FileOffset);//分割得到段和偏移
public:
	MapStructrue* GetMap(char* funcname);
public:
	DWORD					dwRO_first_section;
	IMAGE_DOS_HEADER		image_dos_header;
	char					*reservedheader;
	IMAGE_NT_HEADERS		image_nt_headers;
	IMAGE_SECTION_HEADER	image_section_header[MAX_SECTION_NUM];
	char					*image_section[MAX_SECTION_NUM];
	BOOL OpenFileName(char* FileName);
	void UpdateHeaders(BOOL bSaveAndValidate);							   //更新PE头
	void UpdateHeadersSections(BOOL bSaveAndValidate);					   //更新PE节
	DWORD PEAlign(DWORD dwTarNum,DWORD dwAlignTo);						   //PE节对齐
	PIMAGE_SECTION_HEADER AddSection(char* Base,int len,char* SectionName);//添加节
	DWORD GetNewSection();												   //获得新节的VirtualAddress地址
	void Init();
	void Free();
	void FreeMapVector();
	void MakePE(char* filename,int len);//制造一个PE
};
