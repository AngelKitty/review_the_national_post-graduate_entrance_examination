
/*-----------------------------------------------------------------------
第17章  软件保护技术
 memcrc32.cpp -- 内存映像校验，仅校验代码区块
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h>

DWORD CRC32(BYTE*,DWORD);
BOOL CodeSectionCRC32( );
	


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	
	if(CodeSectionCRC32())
		MessageBox(NULL,TEXT ("CRC32 Check OK!"),TEXT ("OK"),MB_ICONEXCLAMATION);
	else
    		MessageBox(NULL,"File corrupted! !","CRC error",MB_ICONEXCLAMATION);
	return 0;
}

////////////////////////////////////////////////////////////////
// 计算代码区块的CRC32值
//

BOOL CodeSectionCRC32( )
{

	PIMAGE_DOS_HEADER	    pDosHeader=NULL;
    PIMAGE_NT_HEADERS       pNtHeader=NULL;
    PIMAGE_SECTION_HEADER   pSecHeader=NULL;

	DWORD					ImageBase,OriginalCRC32;


	ImageBase=(DWORD)GetModuleHandle(NULL);//取基址，其实本例也可直接用0x4000000这个值

	 pDosHeader=(PIMAGE_DOS_HEADER)ImageBase;	 	 
     pNtHeader=(PIMAGE_NT_HEADERS32)((DWORD)pDosHeader+pDosHeader->e_lfanew);
	 //定位到PE文件头（即字串“PE\0\0”处）前4个字节处，并读出储存在这里的CRC-32值：
	 OriginalCRC32 =*((DWORD *)((DWORD)pNtHeader-4));

	 pSecHeader=IMAGE_FIRST_SECTION(pNtHeader);   //得到第一个区块的起始地址  
	
	 //假设第一个区块就是代码区块
	 //是从磁盘文件得到代码块的地址和大小来计算CRC32值
	 if(OriginalCRC32==CRC32((BYTE*) (ImageBase+pSecHeader->VirtualAddress),pSecHeader->Misc.VirtualSize)) 
		return TRUE;
	else
		return FALSE;


	//如果程序从磁盘文件获取第一区块的数据，加壳后，数据将可能不准确，因此对于要加壳的程序来说，
	//直接填上代码区块的地址和大小，方法是先随便填一数据，编译后用PE工具查看生成文件的代码区块的RVA和大小，
	//再填进来重新编译
	/*
	if(OriginalCRC32==CRC32((BYTE*) 0x401000,0x36AE)) 
		return TRUE;
	else
		return FALSE;
*/

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
