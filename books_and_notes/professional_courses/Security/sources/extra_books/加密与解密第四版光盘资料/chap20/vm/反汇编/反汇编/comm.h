
//#include "..\ByteCode\ByteCode\CodeAnalyse.h"

#define VMPACKERTITLE             "VMPacker"

//MessageBox
void MsgBox( char* content, UINT nType = MB_OK );

//去掉换行符和空格
//返回值,当没有内容时返回NULL;
BOOL Trim( char* pstr,char* str );

//字符串转16进制数
DWORD StringToHex(char* strSource);
void OutPutStr(char *str, ...);

struct MapStructrue
{
	WORD  Segment;                   //PE段
	DWORD Offset;                    //偏移
	char  SymbleName[512];           //符号名称
	DWORD VirtualAddress;            //虚拟地址
	char  LibObject[512];            //输入文件名称
	//CLink Codelink;					 //代码链
	MapStructrue()
	{
		Segment = 0;
		Offset = 0;
		VirtualAddress = 0;
		memset(SymbleName,0,512);
		memset(LibObject,0,512);
		//Codelink.RemoveAll();
	}
};