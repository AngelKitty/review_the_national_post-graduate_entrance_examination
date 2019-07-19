
/*-----------------------------------------------------------------------
第17章  软件保护技术
 DetectingBk.cpp -- 通过CRC检查断点
  计算begindecrypt与enddecrypt之间的CRC值，如在这段代码设bpx断点，将会被发现
《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
-----------------------------------------------------------------------*/

#include <windows.h>

DWORD CRC32(BYTE*,DWORD);
	

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	DWORD address1,address2,size;

	_asm mov address1,offset begindecrypt;
	_asm mov address2,offset enddecrypt;

	begindecrypt : //标记代码的起妈地址
	MessageBox(NULL,TEXT ("Hello world!"),TEXT ("OK"),MB_ICONEXCLAMATION);
	enddecrypt ://标记代码的结束地址

	size=address2-address1;	
	if(CRC32((BYTE*)address1,size)==0x78E888AE) //0x78E888AE的值，可以通过SoftICE等调试工具跟踪得到，再填进来重新编译
		MessageBox(NULL,TEXT ("Breakpoint not found!"),TEXT ("Breakpoint detection by CRC"),MB_ICONEXCLAMATION);
	else
    		MessageBox(NULL,"Breakpoint or a change in the program found !","Breakpoint detection by CRC",MB_ICONEXCLAMATION);
	return 0;
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
