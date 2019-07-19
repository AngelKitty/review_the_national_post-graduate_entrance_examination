/*-----------------------------------------------------------------------
第17章  软件保护技术
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h>

int patch();
DWORD CRC32(BYTE*,DWORD);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	
	patch();
	
	return 0;
}

//////////////////////////////////////////////////////////////////////



int patch()
{
	PIMAGE_DOS_HEADER	    pDosHeader=NULL;
    PIMAGE_NT_HEADERS       pNtHeader=NULL;
    PIMAGE_SECTION_HEADER   pSecHeader=NULL;
	
	TCHAR szFileName[MAX_PATH ] ;
	DWORD szTemp;
	static OPENFILENAME ofn ;
	HANDLE hFile;	
	long FileSize;
	DWORD szCRC32;
	TCHAR  *pBuffer ;

	ZeroMemory(&ofn, sizeof(ofn));                             // 初始化OPENFILENAME结构

	static TCHAR szFilter[] =TEXT ("EXE Files (*.exe)\0*.exe\0") \
		TEXT ("All Files (*.*)\0*.*\0\0") ;
	
	szFileName[0] = '\0';                         
	ZeroMemory(&ofn, sizeof(ofn));                             // 初始化OPENFILENAME结构
	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.lpstrFilter       = szFilter ;
	ofn.lpstrFile         = szFileName ;         
	ofn.nMaxFile          = MAX_PATH ;   
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;           
	
	if(GetOpenFileName (&ofn))
	{
		
		hFile = CreateFile(
			szFileName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ, 
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		
		
		if( hFile != INVALID_HANDLE_VALUE )
		{
			FileSize=GetFileSize(hFile,&szTemp);  // 获取文件大小
			if (FileSize == 0xFFFFFFFF) return FALSE;
			pBuffer = new TCHAR [FileSize];     // 申请内存
			if(ReadFile(hFile, pBuffer, FileSize, &szTemp, NULL)==NULL) return FALSE;// 读取数据
			
		}
		else
		{
			MessageBox(NULL,"I can't access file!","CRC error",MB_ICONEXCLAMATION);
			return 0;
		}
		
	}
	else
		return 0;
	
	
	pDosHeader=(PIMAGE_DOS_HEADER)pBuffer;	 	 
	pNtHeader=(PIMAGE_NT_HEADERS32)((DWORD)pDosHeader+pDosHeader->e_lfanew);
	pSecHeader=IMAGE_FIRST_SECTION(pNtHeader);   //得到第一个区块的起始地址  
	szCRC32=CRC32((BYTE*)(pBuffer+pSecHeader->PointerToRawData),pSecHeader->Misc.VirtualSize);
	
	DWORD Writeadd=DWORD(pDosHeader->e_lfanew-4);//定位到PE文件头（即字串“PE\0\0”处）前4个字节处，准备在这写CRC-32值：
	SetFilePointer(hFile,Writeadd,NULL,FILE_BEGIN);//设置文件指针
	
	if(!WriteFile(hFile,&szCRC32,4,&szTemp,NULL))
	{
		MessageBox(NULL,"Error while patching !","Patch aborted",MB_ICONEXCLAMATION);
		CloseHandle(hFile);
		return 0;
	}
	
	
	delete pBuffer;  // 释放内存
	CloseHandle(hFile);	
	MessageBox(NULL,"Patch successfull !","Patch",MB_ICONINFORMATION);
	return 1;
	
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