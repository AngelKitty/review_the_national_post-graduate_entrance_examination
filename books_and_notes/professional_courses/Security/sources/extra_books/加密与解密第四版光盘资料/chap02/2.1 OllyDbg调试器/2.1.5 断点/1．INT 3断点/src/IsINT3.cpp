/*-----------------------------------------------------------------------
IsINT3.cpp -- 检测CC断点

《加密与解密（第四版）》
(c)  看雪学院 www.kanxue.com 2000-2018
----------------------------------------------

#include <windows.h>

BOOL IsCC();


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{

	if( IsCC() )
		MessageBox(NULL,TEXT ("Found Debug!"),TEXT ("OK"),MB_ICONEXCLAMATION);
	else
    	MessageBox(NULL,TEXT ("No Debug!"),TEXT ("Error"),MB_ICONEXCLAMATION);

	return 0;
}

//////////////////////////////////////////////////////////////////////



BOOL IsCC()
{
	FARPROC Uaddr ;
	BYTE Mark = 0;
	(FARPROC&) Uaddr =GetProcAddress ( LoadLibrary("user32.dll"),"MessageBoxA");
	Mark = *((BYTE*)Uaddr);   // 取MessageBoxA函数第一字节
	if(Mark ==0xCC)           // 如该字节为CC，则认为MessageBoxA函数被下断
		return TRUE;
	else	
		return FALSE;

}