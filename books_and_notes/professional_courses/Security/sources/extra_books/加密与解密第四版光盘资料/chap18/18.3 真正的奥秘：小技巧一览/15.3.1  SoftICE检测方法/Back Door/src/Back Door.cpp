/*--------------------------------------------------------------
   Back Door.cpp -- 利用SoftICE后门指令检测SoftICE
                           www.pediy.com (c) 段钢, 2003
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
//
// 利用SoftICE后门指令检测SoftICE，如SoftICE不存在将引起断点异常。
//////////////////////////////////////////////////////////////////////

BOOL SoftICELoaded()
{
	
	try
	{
		_asm {
			mov si, 'FG'  // 魔法值"FG"
			mov di, 'JM'  // 魔法值"JM"		
			int 3
			nop
			jmp		label
			}
label:
		return TRUE;
	}
    catch (...)
	{
		return FALSE;
	}

}

