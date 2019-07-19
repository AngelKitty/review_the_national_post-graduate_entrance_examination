/*-----------------------------------------------------------------------
第17章  软件保护技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/


#include <windows.h>

BOOL IsFileModified();
DWORD CRC32(BYTE*,DWORD);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{

	if(IsFileModified() )
		MessageBox(NULL,TEXT ("CRC32 Check OK!"),TEXT ("OK"),MB_ICONEXCLAMATION);
	else
    	MessageBox(NULL,"File corrupted! !","CRC error",MB_ICONEXCLAMATION);
	return 0;
}

////////////////////////////////////////////////////////////////
// 打开文件判断CRC32值是否正确
//

BOOL IsFileModified()
{
	PIMAGE_DOS_HEADER	    pDosHeader=NULL;
    PIMAGE_NT_HEADERS       pNtHeader=NULL;
    PIMAGE_SECTION_HEADER   pSecHeader=NULL;

	DWORD fileSize,OriginalCRC32,NumberOfBytesRW;
 	TCHAR  *pBuffer ,szFileName[MAX_PATH]; 


	//获得文件名
        GetModuleFileName(NULL,szFileName,MAX_PATH);
	//打开文件
	HANDLE hFile = CreateFile(
		szFileName,
		GENERIC_READ,
		FILE_SHARE_READ, 
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	 if ( hFile == INVALID_HANDLE_VALUE ) return FALSE;


	//获得文件长度 :
	fileSize = GetFileSize(hFile,NULL);
	if (fileSize == 0xFFFFFFFF) return FALSE;

	pBuffer = new TCHAR [fileSize];     // 申请内存,也可用VirtualAlloc等函数申请内存
	ReadFile(hFile,pBuffer, fileSize, &NumberOfBytesRW, NULL);//读取文件内容
	CloseHandle(hFile);  //关闭文件
		

	pDosHeader=(PIMAGE_DOS_HEADER)pBuffer;
	pNtHeader=(PIMAGE_NT_HEADERS32)((DWORD)pDosHeader+pDosHeader->e_lfanew);
	 //定位到PE文件头（即字串“PE\0\0”处）前4个字节处，并读出储存在这里的CRC-32值：
	OriginalCRC32 =*((DWORD *)((DWORD)pNtHeader-4));	 	 

	fileSize=fileSize-DWORD(pDosHeader->e_lfanew);//将PE文件头前那部分数据去除

	if (CRC32((BYTE*)(pBuffer+pDosHeader->e_lfanew),fileSize) == OriginalCRC32 )
		return TRUE;
	else
		return FALSE;

}
////////////////////////////////////////////////////////////////
// 计算字符串的CRC32值
// 参数：欲计算CRC32值字符串的首地址和大小
// 返回值: 返回CRC32值

DWORD CRC32(BYTE* ptr,DWORD Size)
{

	DWORD crcTable[256],crcTmp1;
	
	//动态生成CRC-32表
	for (int i=0; i<256; i++)
	 {
		crcTmp1 = i;
		for (int j=8; j>0; j--)
		 {
			if (crcTmp1&1) crcTmp1 = (crcTmp1 >> 1) ^ 0xEDB88320L;
			 else crcTmp1 >>= 1;
		}

		 crcTable[i] = crcTmp1;
	 }
	//计算CRC32值
	DWORD crcTmp2= 0xFFFFFFFF;
	while(Size--)
	{
		crcTmp2 = ((crcTmp2>>8) & 0x00FFFFFF) ^ crcTable[ (crcTmp2^(*ptr)) & 0xFF ];
		ptr++;
	}
		
	return (crcTmp2^0xFFFFFFFF);
}
