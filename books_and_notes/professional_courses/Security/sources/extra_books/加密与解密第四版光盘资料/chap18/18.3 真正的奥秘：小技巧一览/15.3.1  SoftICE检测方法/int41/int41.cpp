/*--------------------------------------------------------------
   int41.cpp -- 利用int41 检测SoftICE
                           (c) www.pediy.com 段钢, 2003
  --------------------------------------------------------------*/

#include <windows.h>

BOOL SoftICELoaded();
#define MAX_HANDLE_LIST_BUF 0x20000

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
//利用int41 检测SoftICE
//////////////////////////////////////////////////////////////////////

BOOL SoftICELoaded()
{
	try {
    _asm {
         mov eax,0x4f          // AX = 004Fh
         int 0x41              // INT 41 
         cmp ax,0x0F386        // 如果调试器存在AX = F386h 
         jz label
	   }

		return FALSE;
label:
		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}

}


