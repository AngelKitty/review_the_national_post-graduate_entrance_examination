/*--------------------------------------------------------------
   UnhandledExceptionFilter.cpp -- 利用UnhandledExceptionFilter检测SoftICE
                           (c) www.pediy.com 段钢, 2003
  --------------------------------------------------------------*/

#include <windows.h>

BOOL SoftICELoaded();


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{

	if( SoftICELoaded() )
		MessageBox(NULL,TEXT ("SoftICE is active!"),TEXT ("OK"),MB_ICONEXCLAMATION);
	else
    	MessageBox(NULL,TEXT ("Can't find SoftICE with this method!"),TEXT ("Error"),MB_ICONEXCLAMATION);

	return 0;
}

//////////////////////////////////////////////////////////////////////



BOOL SoftICELoaded()
{

	FARPROC Uaddr ;
	BYTE Mark = 0;
	(FARPROC&) Uaddr =GetProcAddress ( GetModuleHandle("kernel32.dll"),"UnhandledExceptionFilter");
	Mark = *((BYTE*)Uaddr);   // 取UnhandledExceptionFilter函数第一字节
	if(Mark ==0xCC)           // 如该字节为CC，则SoftICE己加载
		return TRUE;
	else	
		return FALSE;

}